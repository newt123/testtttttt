/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

# include "jam.h"
# include "execcmd.h"
# include "lists.h"
# include <errno.h>

# ifdef unix

# if defined( _AIX) || \
	(defined (COHERENT) && defined (_I386)) || \
	defined(__sgi) || \
	defined(M_XENIX)
# define vfork() fork()
# endif

/*
 * execunix.c - execute a shell script on UNIX
 *
 * If $(JAMSHELL) is defined, uses that to formulate execvp().
 * The default is:
 *
 *	/bin/sh -c %
 *
 * Each word must be an individual element in a jam variable value.
 *
 * In $(JAMSHELL), % expands to the command string and ! expands to 
 * the slot number (starting at 1) for multiprocess (-j) invocations.
 * If $(JAMSHELL) doesn't include a %, it is tacked on as the last
 * argument.
 *
 * Don't just set JAMSHELL to /bin/sh - it won't work!
 *
 * External routines:
 *	execcmd() - launch an async command execution
 * 	execwait() - wait and drive at most one execution completion
 *
 * Internal routines:
 *	onintr() - bump intr to note command interruption
 *
 * 04/08/94 (seiwald) - Coherent/386 support added.
 * 05/04/94 (seiwald) - async multiprocess interface
 * 01/22/95 (seiwald) - $(JAMSHELL) support
 */

static int intr = 0;

static int cmdsrunning = 0;

static void (*istat)();

static struct
{
	int	pid;
	void	(*func)();
	void 	*closure;
} cmdtab[ MAXJOBS ] = {0};

/*
 * onintr() - bump intr to note command interruption
 */

void
onintr()
{
	intr++;
	printf( "...interrupted\n" );
}

/*
 * execcmd() - launch an async command execution
 */

void
execcmd( string, func, closure, shell )
char *string;
void (*func)();
void *closure;
LIST *shell;
{
	int pid;
	int slot;
	char *argv[ MAXARGC + 1 ];	/* +1 for NULL */

	/* Find a slot in the running commands table for this one. */

	for( slot = 0; slot < MAXJOBS; slot++ )
	    if( !cmdtab[ slot ].pid )
		break;

	if( slot == MAXJOBS )
	{
	    printf( "no slots for child!\n" );
	    exit( EXITBAD );
	}

	/* Forumulate argv */
	/* If shell was defined, be prepared for % and ! subs. */
	/* Otherwise, use stock /bin/sh. */

	if( shell )
	{
	    int i;
	    char jobno[4];
	    int gotpercent = 0;

	    sprintf( jobno, "%d", slot + 1 );

	    for( i = 0; shell && i < MAXARGC; i++, shell = list_next( shell ) )
	    {
		switch( shell->string[0] )
		{
		case '%':	argv[i] = string; gotpercent++; break;
		case '!':	argv[i] = jobno; break;
		default:	argv[i] = shell->string;
		}
		if( DEBUG_EXECCMD )
		    printf( "argv[%d] = '%s'\n", i, argv[i] );
	    }

	    if( !gotpercent )
		argv[i++] = string;

	    argv[i] = 0;
	}
	else
	{
	    argv[0] = "/bin/sh";
	    argv[1] = "-c";
	    argv[2] = string;
	    argv[3] = 0;
	}

	/* Catch interrupts whenever commands are running. */

	if( !cmdsrunning++ )
	    istat = signal( SIGINT, onintr );

	/* Start the command */

	if ((pid = vfork()) == 0) 
   	{
		execvp( argv[0], argv );
		_exit(127);
	}

	if( pid == -1 )
	{
	    perror( "vfork" );
	    exit( EXITBAD );
	}

	/* Save the operation for execwait() to find. */

	cmdtab[ slot ].pid = pid;
	cmdtab[ slot ].func = func;
	cmdtab[ slot ].closure = closure;

	/* Wait until we're under the limit of concurrent commands. */
	/* Don't trust globs.jobs alone. */

	while( cmdsrunning >= MAXJOBS || cmdsrunning >= globs.jobs )
	    if( !execwait() )
		break;
}

/*
 * execwait() - wait and drive at most one execution completion
 */

int
execwait()
{
	int i;
	int status, w;
	int rstat;

	/* Handle naive make1() which doesn't know if cmds are running. */

	if( !cmdsrunning )
	    return 0;

	/* Pick up process pid and status */
    
	while( ( w = wait( &status ) ) == -1 && errno == EINTR )
		;

	if( w == -1 )
	{
	    printf( "child process(es) lost!\n" );
	    perror("wait");
	    exit( EXITBAD );
	}

	/* Find the process in the cmdtab. */

	for( i = 0; i < MAXJOBS; i++ )
	    if( w == cmdtab[ i ].pid )
		break;

	if( i == MAXJOBS )
	{
	    printf( "waif child found!\n" );
	    exit( EXITBAD );
	}

	/* Drive the completion */

	if( !--cmdsrunning )
	    signal( SIGINT, istat );

	if( intr )
	    rstat = EXEC_CMD_INTR;
	else if( w == -1 || status != 0 )
	    rstat = EXEC_CMD_FAIL;
	else
	    rstat = EXEC_CMD_OK;

	cmdtab[ i ].pid = 0;

	(*cmdtab[ i ].func)( cmdtab[ i ].closure, rstat );

	return 1;
}

# endif /* unix */
