/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*
 * make1.c - execute command to bring targets up to date
 *
 * This module contains make1(), the entry point called by make() to 
 * recursively decend the dependency graph executing update actions as
 * marked by make0().
 *
 * External routines:
 *
 *	make1() - execute commands to update a TARGET and all its dependents
 *
 * Internal routines, the recursive/asynchronous command executors:
 *
 *	make1a() - recursively traverse target tree, calling make1b()
 *	make1b() - dependents of target built, now build target with make1c()
 *	make1c() - launch target's next command, call make1b() when done
 *	make1d() - handle command execution completion and call back make1c()
 *
 * Internal support routines:
 *
 *	make1cmds() - turn ACTIONS into CMDs, grouping, splitting, etc
 *	make1chunk() - compute number of source that can fit on cmd line
 *	make1list() - turn a list of targets into a LIST, for $(<) and $(>)
 *	make1remove() - remove targets after interrupted command
 *
 * 04/16/94 (seiwald) - Split from make.c.
 * 04/21/94 (seiwald) - Handle empty "updated" actions.
 * 05/04/94 (seiwald) - async multiprocess (-j) support
 * 06/01/94 (seiwald) - new 'actions existing' does existing sources
 * 12/20/94 (seiwald) - NOTIME renamed NOTFILE.
 * 01/19/95 (seiwald) - distinguish between CANTFIND/CANTMAKE targets.
 * 01/22/94 (seiwald) - pass per-target JAMSHELL down to execcmd().
 * 02/28/95 (seiwald) - Handle empty "existing" actions.
 * 03/10/95 (seiwald) - Fancy counts.
 */

# include "jam.h"

# include "lists.h"
# include "parse.h"
# include "variable.h"
# include "rules.h"

# include "search.h"
# include "newstr.h"
# include "make.h"
# include "command.h"
# include "execcmd.h"

static void make1a();
static void make1b();
static void make1c();
static void make1d();

static CMD *make1cmds();
static int make1chunk();
static void make1remove();
static LIST *make1list();
static int make1exec();

/* Ugly static - it's too hard to carry it through the callbacks. */

static struct {
	int	failed;
	int	skipped;
	int	total;
	int	made;
} counts[1] ;

/*
 * make1() - execute commands to update a TARGET and all its dependents
 */

static int intr = 0;

void
make1( t )
TARGET *t;
{
	memset( (char *)counts, 0, sizeof( *counts ) );

	/* Recursively make the target and its dependents */

	make1a( t, (TARGET *)0 );

	/* Wait for any outstanding commands to finish running. */

	while( execwait() )
	    ;

	/* Talk about it */

	if( DEBUG_MAKE && counts->failed )
	    printf( "...failed updating %d target(s)...\n", counts->failed );

	if( DEBUG_MAKE && counts->skipped )
	    printf( "...skipped %d target(s)...\n", counts->skipped );

	if( DEBUG_MAKE && counts->made )
	    printf( "...updated %d target(s)...\n", counts->made );
}

/*
 * make1a() - recursively traverse target tree, calling make1b()
 */

static void
make1a( t, parent )
TARGET	*t;
TARGET	*parent;
{
	TARGETS	*c;
	int i;

	/* If the parent is the first to try to build this target */
	/* or this target is in the make1c() quagmire, arrange for the */
	/* parent to be notified when this target is built. */

	if( parent )
	    switch( t->progress )
	{
	case T_MAKE_INIT:
	case T_MAKE_ACTIVE:
	case T_MAKE_RUNNING:
	    t->parents = targetentry( t->parents, parent );
	    parent->asynccnt++;
	}

	if( t->progress != T_MAKE_INIT )
	    return;

	/* Asynccnt counts the dependents preventing this target from */
	/* proceeding to make1b() for actual building.  We start off with */
	/* a count of 1 to prevent anything from happening until we can */
	/* call all dependents.  This 1 is accounted for when we call */
	/* make1b() ourselves, below. */

	t->asynccnt = 1;

	/* Recurse on our dependents, manipulating progress to guard */
	/* against circular dependency. */

	t->progress = T_MAKE_ONSTACK;

	for( i = T_DEPS_DEPENDS; i <= T_DEPS_INCLUDES; i++ )
	    for( c = t->deps[i]; c && !intr; c = c->next )
		make1a( c->target, t );

	t->progress = T_MAKE_ACTIVE;

	/* Now that all dependents have bumped asynccnt, we now allow */
	/* decrement our reference to asynccnt. */ 

	make1b( t );
}

/*
 * make1b() - dependents of target built, now build target with make1c()
 */

static void
make1b( t )
TARGET	*t;
{
	TARGETS	*c;
	int 	i;
	char 	*failed = "dependents";

	/* If any dependents are still outstanding, wait until they */
	/* call make1b() to signal their completion. */

	if( --t->asynccnt )
	    return;

	/* Now ready to build target 't'... if dependents built ok. */

	/* Collect status from dependents */

	for( i = T_DEPS_DEPENDS; i <= T_DEPS_INCLUDES; i++ )
	    for( c = t->deps[i]; c; c = c->next )
		if( c->target->status > t->status )
	{
	    failed = c->target->name;
	    t->status = c->target->status;
	}

	/* If actions on deps have failed, bail. */
	/* Otherwise, execute all actions to make target */

	if( t->status == EXEC_CMD_FAIL && t->actions )
	{
	    ++counts->skipped;
	    printf( "...skipped %s for lack of %s...\n", t->name, failed );
	}

	if( t->status == EXEC_CMD_OK )
	    switch( t->fate )
	{
	case T_FATE_INIT:
	case T_FATE_MAKING:
	    /* shouldn't happen */

	case T_FATE_STABLE:
	    break;

	case T_FATE_CANTFIND:
	case T_FATE_CANTMAKE:
	    t->status = EXEC_CMD_FAIL;
	    break;

	case T_FATE_ISTMP:
	    if( DEBUG_MAKEQ )
		printf( "...using %s...\n", t->name );
	    break;

	case T_FATE_TOUCHED:
	case T_FATE_MISSING:
	case T_FATE_OUTDATED:
	case T_FATE_UPDATE:
	    /* Set "on target" vars, build actions, unset vars */
	    /* Set "progress" so that make1c() counts this target among */
	    /* the successes/failures. */

	    if( t->actions )
	    {
		if( DEBUG_MAKE && !( ++counts->total % 100 ) )
		    printf( "...on %dth target...\n", counts->total );

		pushsettings( t->settings );
		t->cmds = (char *)make1cmds( t->actions );
		popsettings( t->settings );

		t->progress = T_MAKE_RUNNING;
	    }

	    break;
	}

	/* Call make1c() to begin the execution of the chain of commands */
	/* needed to build target.  If we're not going to build target */
	/* (because of dependency failures or because no commands need to */
	/* be run) the chain will be empty and make1c() will directly */
	/* signal the completion of target. */

	make1c( t );
}

/*
 * make1c() - launch target's next command, call make1b() when done
 */

static void
make1c( t )
TARGET	*t;
{
	CMD	*cmd = (CMD *)t->cmds;

	/* If there are (more) commands to run to build this target */
	/* (and we haven't hit an error running earlier comands) we */
	/* launch the command with execcmd(). */
	
	/* If there are no more commands to run, we collect the status */
	/* from all the actions then report our completion to all the */
	/* parents. */

	if( cmd && t->status == EXEC_CMD_OK )
	{
	    if( cmd->rule->flags & RULE_QUIETLY ? DEBUG_MAKEQ : DEBUG_MAKE )
	    {
		printf( "%s ", cmd->rule->name );
		list_print( cmd->targets );
		printf( "\n" );
	    }

	    if( DEBUG_EXEC )
		printf( "%s\n", cmd->buf );

	    if( DEBUG_MAKE && !globs.noexec )
		fflush( stdout );
	    
	    if( globs.noexec )
	    {
		make1d( t, EXEC_CMD_OK );
	    } 
	    else 
	    {
		execcmd( cmd->buf, make1d, t, cmd->shell );
	    }
	}
	else
	{
	    TARGETS	*c;
	    ACTIONS	*actions;

	    /* Collect status from actions, and distribute it as well */

	    for( actions = t->actions; actions; actions = actions->next )
		if( actions->action->status > t->status )
		    t->status = actions->action->status;

	    for( actions = t->actions; actions; actions = actions->next )
		if( t->status > actions->action->status )
		    actions->action->status = t->status;

	    /* Tally success/failure for those we tried to update. */

	    if( t->progress == T_MAKE_RUNNING )
		switch( t->status )
	    {
	    case EXEC_CMD_OK:
		++counts->made;
		break;
	    case EXEC_CMD_FAIL:
		++counts->failed;
		break;
	    }

	    /* Tell parents dependent has been built */

	    t->progress = T_MAKE_DONE;

	    for( c = t->parents; c; c = c->next )
		make1b( c->target );
	}
}

/*
 * make1d() - handle command execution completion and call back make1c()
 */

static void
make1d( t, status )
TARGET	*t;
int	status;
{
	CMD	*cmd = (CMD *)t->cmds;

	/* Execcmd() has completed.  All we need to do is fiddle with the */
	/* status and signal our completion so make1c() can run the next */
	/* command.  On interrupts, we bail heavily. */

	if( status == EXEC_CMD_FAIL && ( cmd->rule->flags & RULE_IGNORE ) )
	    status = EXEC_CMD_OK;

	/* On interrupt, set intr so _everything_ fails */

	if( status == EXEC_CMD_INTR )
	    ++intr;

	if( status == EXEC_CMD_FAIL && DEBUG_MAKE )
	{
	    printf( "...failed %s ", cmd->rule->name );
	    list_print( cmd->targets );
	    printf( "...\n" );
	}

	/* If the command was interrupted or failed and the target */
	/* is not "precious", remove the targets */

	if( status != EXEC_CMD_OK && !( cmd->rule->flags & RULE_TOGETHER ) )
	    make1remove( cmd->targets );

	t->status = status;
	t->cmds = (char *)cmd_next( cmd );

	cmd_free( cmd );

	make1c( t );
}

/*
 * make1cmds() - turn ACTIONS into CMDs, grouping, splitting, etc
 *
 * Essentially copies a chain of ACTIONs to a chain of CMDs, 
 * grouping RULE_TOGETHER actions, splitting RULE_PIECEMEAL actions,
 * and handling RULE_NEWSRCS actions.  The result is a chain of
 * CMDs which can be expanded by var_string() and executed with
 * execcmd().
 */

static CMD *
make1cmds( a0 )
ACTIONS	*a0;
{
	CMD *cmds = 0;
	LIST *shell = var_get( "JAMSHELL" );	/* shell is per-target */

	/* Step through actions */
	/* Actions may be shared with other targets or grouped with */
	/* RULE_TOGETHER, so actions already seen are skipped. */

	for( ; a0; a0 = a0->next )
	{
	    RULE    *rule = a0->action->rule;
	    int	    chunk = 0;
	    LIST    *nt, *ns;
	    ACTIONS *a1;

	    /* Only do rules with commands to execute. */
	    /* If this action has already been executed, use saved status */

	    if( !rule->actions || a0->action->running )
		continue;

	    a0->action->running = 1;
	    
	    /* Make LISTS of targets and sources */
	    /* If `execute together` has been specified for this rule, tack */
	    /* on sources from each instance of this rule for this target. */

	    nt = make1list( L0, a0->action->targets, 0 );
	    ns = make1list( L0, a0->action->sources, rule->flags );

	    if( rule->flags & RULE_TOGETHER )
		for( a1 = a0->next; a1; a1 = a1->next )
		    if( a1->action->rule == rule && !a1->action->running )
	    {
		ns = make1list( ns, a1->action->sources, rule->flags );
		a1->action->running = 1;
	    }

	    /* If doing only updated (or existing) sources, but none have */
	    /* been updated (or exist), skip this action. */

	    if( !ns && ( rule->flags & ( RULE_NEWSRCS | RULE_EXISTING ) ) )
	    {
		list_free( nt );
		continue;
	    }

	    /* If rule is to be cut into (at most) MAXCMD pieces, estimate */
	    /* bytes per $(>) element and aim for using MAXCMD minus a */
	    /* fudgefactor. */

	    if( rule->flags & RULE_PIECEMEAL )
		chunk = make1chunk( rule->actions, nt, ns );

	    if( chunk )
	    {
		int  start;
		LIST *somes;

		if( DEBUG_EXEC )
		    printf( "%s: %d args per exec\n", rule->name, chunk );

		for( start = 0;
		     somes = list_sublist( ns, start, chunk );
		     start += chunk )
		{
		    cmds = cmd_new( cmds, rule, 
				list_copy( L0, nt ), somes, 
				list_copy( L0, shell ) );
		}

		list_free( nt );
		list_free( ns );
	    }
	    else
	    {
		cmds = cmd_new( cmds, rule, nt, ns, list_copy( L0, shell ) );
	    }
	}

	return cmds;
}

/*
 * make1chunk() - compute number of source that can fit on cmd line
 */

static int
make1chunk( cmd, targets, sources )
char	*cmd;
LIST	*targets;
LIST	*sources;
{
	int onesize;
	int onediff;
	int chunk = 0;
	LIST *somes;
	char buf[ MAXCMD ];

	somes = list_sublist( sources, 0, 1 );
	onesize = var_string( cmd, buf, targets, somes );
	list_free( somes );

	somes = list_sublist( sources, 0, 2 );
	onediff = var_string( cmd, buf, targets, somes ) - onesize;
	list_free( somes );

	if( onediff > 0 )
	    chunk = 3 * ( MAXCMD - onesize ) / 5 / onediff + 1;

	return chunk;
}

/*
 * make1list() - turn a list of targets into a LIST, for $(<) and $(>)
 */

static LIST *
make1list( l, targets, flags )
LIST	*l;
TARGETS	*targets;
int	flags;
{
    for( ; targets; targets = targets->next )
    {
	TARGET *t = targets->target;

	/*
	 * spot the kludge!  If a target is not in the dependency tree,
	 * it didn't get bound by make0(), so we have to do it here.
	 * Ugly.
	 */

	if( t->binding == T_BIND_UNBOUND && !( t->flags & T_FLAG_NOTFILE ) )
	{
	    /* Sources to 'actions existing' are never in the dependency */
	    /* graph (if they were, they'd get built and 'existing' would */
	    /* be superfluous, so throttle warning message about independent */
	    /* targets. */

	    if( !( flags & RULE_EXISTING ) )
		printf( "warning: using independent target %s\n", t->name );

	    pushsettings( t->settings );
	    t->boundname = search( t->name, &t->time );
	    t->binding = t->time ? T_BIND_EXISTS : T_BIND_MISSING;
	    popsettings( t->settings );
	}

	if( ( flags & RULE_EXISTING ) && t->binding != T_BIND_EXISTS )
	    continue;

	if( ( flags & RULE_NEWSRCS ) && t->fate <= T_FATE_STABLE )
	    continue;

	/* boundname is null for T_FLAG_NOTFILE targets - use name */

	l = list_new( l, copystr( t->boundname ? t->boundname : t->name ) );
    }

    return l;
}

/*
 * make1remove() - remove targets after interrupted command
 */

static void
make1remove( targets )
LIST *targets;
{
	for( ; targets; targets = list_next( targets ) )
	{
	    if( !unlink( targets->string ) )
		printf( "...removing %s\n", targets->string );
	}
}
