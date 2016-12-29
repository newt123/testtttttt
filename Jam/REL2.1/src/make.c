/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*
 * make.c - bring a target up to date, once rules are in place
 *
 * This modules controls the execution of rules to bring a target and
 * its dependencies up to date.  It is invoked after the targets, rules,
 * et. al. described in rules.h are created by the interpreting of the
 * jam files.
 *
 * This file contains the main make() entry point and the first pass
 * make0().  The second pass, make1(), which actually does the command
 * execution, is in make1.c.
 *
 * External routines:
 *	make() - make a target, given its name
 *
 * Internal routines:
 * 	make0() - bind and scan everything to make a TARGET
 *
 * 12/26/93 (seiwald) - allow NOTIME targets to be expanded via $(<), $(>)
 * 01/04/94 (seiwald) - print all targets, bounded, when tracing commands
 * 04/08/94 (seiwald) - progress report now reflects only targets with actions
 * 04/11/94 (seiwald) - Combined deps & headers into deps[2] in TARGET.
 * 12/20/94 (seiwald) - NOTIME renamed NOTFILE.
 * 12/20/94 (seiwald) - make0() headers after determining fate of target, so 
 *			that headers aren't seen as dependents on themselves.
 * 01/19/95 (seiwald) - distinguish between CANTFIND/CANTMAKE targets.
 * 02/02/95 (seiwald) - propagate leaf source time for new LEAVES rule.
 * 02/14/95 (seiwald) - NOUPDATE rule means don't update existing target.
 * 08/22/95 (seiwald) - NOUPDATE targets immune to anyhow (-a) flag.
 */

# include "jam.h"

# include "lists.h"
# include "parse.h"
# include "variable.h"
# include "rules.h"

# include "search.h"
# include "newstr.h"
# include "make.h"
# include "headers.h"
# include "command.h"

static void make0();

# ifndef max
# define max( a,b ) ((a)>(b)?(a):(b))
# endif

typedef struct {
	int	temp;
	int	updating;
	int	cantfind;
	int	cantmake;
	int	targets;
	int	made;
} COUNTS ;

static char *target_fate[] = 
{
	"init",
	"making",
	"ok",
	"touched",
	"temp",
	"missing",
	"old",
	"update",
	"nofind",
	"nomake"
} ;

# define spaces(x) ( "                " + 16 - ( x > 16 ? 16 : x ) )

/*
 * make() - make a target, given its name
 */

void
make( n_targets, targets, anyhow )
int	n_targets;
char	**targets;
int	anyhow;
{
	int i;
	COUNTS counts[1];

	memset( (char *)counts, 0, sizeof( *counts ) );

	for( i = 0; i < n_targets; i++ )
	    make0( bindtarget( targets[i] ), (time_t)0, 0, counts, anyhow );

	if( DEBUG_MAKEQ )
	{
	    if( counts->targets )
		printf( "...found %d target(s)...\n", counts->targets );
	}

	if( DEBUG_MAKE )
	{
	    if( counts->temp )
		printf( "...using %d temp target(s)...\n", counts->temp );
	    if( counts->updating )
		printf( "...updating %d target(s)...\n", counts->updating );
	    if( counts->cantfind )
		printf( "...can't find %d target(s)...\n", counts->cantfind );
	    if( counts->cantmake )
		printf( "...can't make %d target(s)...\n", counts->cantmake );
	}

	for( i = 0; i < n_targets; i++ )
	    make1( bindtarget( targets[i] ), counts );
}

/*
 * make0() - bind and scan everything to make a TARGET
 *
 * Make0() recursively binds a target, searches for #included headers,
 * calls itself on those headers, and calls itself on any dependents.
 */

static void
make0( t, parent, depth, counts, anyhow )
TARGET	*t;
time_t	parent;
int	depth;
COUNTS	*counts;
int	anyhow;
{
	TARGETS	*c;
	int	fate, hfate;
	time_t	last, leaf, hlast, hleaf;
	char	*flag = "";

	if( DEBUG_MAKEPROG )
	    printf( "make\t--\t%s%s\n", spaces( depth ), t->name );

	/* 
	 * Step 1: don't remake if already trying or tried 
	 */

	switch( t->fate )
	{
	case T_FATE_MAKING:
	    printf( "warning: %s depends on itself\n", t->name );
	    return;

	default:
	    return;

	case T_FATE_INIT:
	    break;
	}

	t->fate = T_FATE_MAKING;

	/*
	 * Step 2: under the influence of "on target" variables,
	 * bind the target and search for headers.
	 */

	/* Step 2a: set "on target" variables. */

	pushsettings( t->settings );

	/* Step 2b: find and timestamp the target file (if it's a file). */

	if( t->binding == T_BIND_UNBOUND && !( t->flags & T_FLAG_NOTFILE ) )
	{
	    t->boundname = search( t->name, &t->time );
	    t->binding = t->time ? T_BIND_EXISTS : T_BIND_MISSING;
	}

	/* If temp file doesn't exist, use parent */

	if( t->binding == T_BIND_MISSING && t->flags & T_FLAG_TEMP && parent )
	{
	    t->time = parent;
	    t->binding = t->time ? T_BIND_PARENTS : T_BIND_MISSING;
	}

	/* Step 2c: If its a file, search for headers. */

	if( t->binding == T_BIND_EXISTS )
	    headers( t );

	/* Step 2d: reset "on target" variables */

	popsettings( t->settings );

	/* 
	 * Step 3: recursively make0() dependents 
	 */

	last = 0;
	leaf = 0;
	fate = T_FATE_STABLE;

	for( c = t->deps[ T_DEPS_DEPENDS ]; c; c = c->next )
	{
	    make0( c->target, t->time, depth + 1, counts, anyhow );
	    leaf = max( leaf, c->target->leaf );
	    leaf = max( leaf, c->target->hleaf );

	    /* If LEAVES has been applied, we only heed the timestamps of */
	    /* the leaf source nodes. */

	    if( t->flags & T_FLAG_LEAVES )
	    {
		last = leaf;
		continue;
	    }

	    last = max( last, c->target->time );
	    last = max( last, c->target->htime );
	    fate = max( fate, c->target->fate );
	    fate = max( fate, c->target->hfate );
	}

	/* If a NOUPDATE file exists, make dependents eternally old. */

	if( t->flags & T_FLAG_NOUPDATE )
	{
	    last = 0;
	    t->time = 0;
	}

	/* Step 3b: determine fate: rebuild target or what? */

	/* 
	    In English:
		If can't find or make child, can't make target.
		If children changed, make target.
		If target missing, make it.
		If children newer, make target.
		If temp's children newer, make temp.
		If deliberately touched, make it.
		If up-to-date temp file present, use it.
	*/

	if( fate >= T_FATE_CANTFIND )
	{
	    fate = T_FATE_CANTMAKE;
	}
	else if( fate > T_FATE_STABLE )
	{
	    fate = T_FATE_UPDATE;
	}
	else if( t->binding == T_BIND_MISSING )
	{
	    fate = T_FATE_MISSING;
	}
	else if( t->binding == T_BIND_EXISTS && last > t->time )
	{
	    fate = T_FATE_OUTDATED;
	}
	else if( t->binding == T_BIND_PARENTS && last > t->time )
	{
	    fate = T_FATE_OUTDATED;
	}
	else if( t->flags & T_FLAG_TOUCHED )
	{
	    fate = T_FATE_TOUCHED;
	}
	else if( anyhow && !( t->flags & T_FLAG_NOUPDATE ) )
	{
	    fate = T_FATE_TOUCHED;
	}
	else if( t->binding == T_BIND_EXISTS && t->flags & T_FLAG_TEMP )
	{
	    fate = T_FATE_ISTMP;
	}

	/* Step 3c: handle missing files */
	/* If it's missing and there are no actions to create it, boom. */
	/* If we can't make a target we don't care about, 'sokay */
	/* We could insist that there are updating actions for all missing */
	/* files, but if they have dependents we just pretend it's NOTFILE. */

	if( fate == T_FATE_MISSING && 
		!t->actions && 
		!t->deps[ T_DEPS_DEPENDS ] )
	{
	    if( t->flags & T_FLAG_NOCARE )
	    {
		fate = T_FATE_STABLE;
	    }
	    else
	    {
		printf( "don't know how to make %s\n", t->name );

		fate = T_FATE_CANTFIND;
	    }
	}

	/* Step 3d: propagate dependents' time & fate. */
	/* Set leaf time to be our time only if this is a leaf. */

	t->time = max( t->time, last );
	t->leaf = leaf ? leaf : t->time ;
	t->fate = fate;

	/*
	 * Step 4: Recursively make0() headers.
	 */

	/* Step 4a: recursively make0() headers */

	hlast = 0;
	hleaf = 0;
	hfate = T_FATE_STABLE;

	for( c = t->deps[ T_DEPS_INCLUDES ]; c; c = c->next )
	{
	    make0( c->target, parent, depth + 1, counts, anyhow );
	    hlast = max( hlast, c->target->time );
	    hlast = max( hlast, c->target->htime );
	    hleaf = max( hleaf, c->target->leaf );
	    hleaf = max( hleaf, c->target->hleaf );
	    hfate = max( hfate, c->target->fate );
	    hfate = max( hfate, c->target->hfate );
	}

	/* Step 4b: propagate dependents' time & fate. */

	t->htime = hlast;
	t->hleaf = hleaf ? hleaf : t->htime;
	t->hfate = hfate;

	/* 
	 * Step 5: a little harmless tabulating for tracing purposes 
	 */

	if( !( ++counts->targets % 1000 ) && DEBUG_MAKE )
	    printf( "...patience...\n" );

	if( fate == T_FATE_ISTMP )
	    counts->temp++;
	else if( fate == T_FATE_CANTFIND )
	    counts->cantfind++;
	else if( fate == T_FATE_CANTMAKE && t->actions )
	    counts->cantmake++;
	else if( fate > T_FATE_STABLE && fate < T_FATE_CANTFIND && t->actions )
	    counts->updating++;

	if( !( t->flags & T_FLAG_NOTFILE ) && fate > T_FATE_STABLE )
	    flag = "+";
	else if( t->binding == T_BIND_EXISTS && parent && t->time > parent )
	    flag = "*";

	if( DEBUG_MAKEPROG )
	    printf( "make%s\t%s\t%s%s\n", 
		flag, target_fate[ t->fate ], 
		spaces( depth ), t->name );
}

