/*
 * /+\
 * +\	Copyright 1993, 1996 Christopher Seiwald.
 * \+/
 *
 * This file is part of jam.
 *
 * License is hereby granted to use this software and distribute it
 * freely, as long as this copyright notice is retained and modifications 
 * are clearly marked.
 *
 * ALL WARRANTIES ARE HEREBY DISCLAIMED.
 */

# include "jam.h"
# include "option.h"
# include "make.h"
# ifdef FATFS
# include "patchlev.h"
# else
# include "patchlevel.h"
# endif

/* These get various function declarations. */

# include "lists.h"
# include "parse.h"
# include "variable.h"
# include "rules.h"
# include "newstr.h"
# include "scan.h"
# ifdef FATFS
# include "timestam.h"
# else
# include "timestamp.h"
# endif

/*
 * jam.c - make redux
 *
 * See jam(1) and Jamfile(5) for usage information.
 *
 * These comments document the code.
 *
 * The top half of the code is structured such:
 *
 *                       jam 
 *                      / | \
 *                 +---+  |  \
 *                /       |   \
 *         jamgram     option  \
 *        /  |   \              \
 *       /   |    \              \
 *      /    |     \             |
 *  scan     |     compile      make
 *           |    /    \       / |  \
 *           |   /      \     /  |   \
 *           |  /        \   /   |    \
 *         parse         rules  search make1
 *                               |	|   \
 *                               |	|    \
 *                               |	|     \
 *                           timestamp command execute
 *
 *
 * The support routines are called by all of the above, but themselves
 * are layered thus:
 *
 *                     variable|expand
 *                      /  |   |   |
 *                     /   |   |   |
 *                    /    |   |   |
 *                 lists   |   |   filesys
 *                    \    |   |
 *                     \   |   |
 *                      \  |   |
 *                     newstr  |
 *                        \    |
 *                         \   |
 *                          \  |
 *                          hash
 *
 * Roughly, the modules are:
 *
 *	command.c - maintain lists of commands
 *	compile.c - compile parsed jam statements
 *	execunix.c - execute a shell script on UNIX
 *	execvms.c - execute a shell script, ala VMS
 *	expand.c - expand a buffer, given variable values
 *	fileunix.c - manipulate file names and scan directories on UNIX
 *	filevms.c - manipulate file names and scan directories on VMS
 *	hash.c - simple in-memory hashing routines 
 *	headers.c - handle #includes in source files
 *	lists.c - maintain lists of strings
 *	make.c - bring a target up to date, once rules are in place
 *	make1.c - execute command to bring targets up to date
 *	newstr.c - string manipulation routines
 *	option.c - command line option processing
 *	parse.c - make and destroy parse trees as driven by the parser
 *	regexp.c - Henry Spencer's regexp
 *	rules.c - access to RULEs, TARGETs, and ACTIONs
 *	scan.c - the jam yacc scanner
 *	search.c - find a target along $(SEARCH) or $(LOCATE) 
 *	timestamp.c - get the timestamp of a file or archive member
 *	variable.c - handle jam multi-element variables
 *	jamgram.yy - jam grammar
 *
 * 05/04/94 (seiwald) - async multiprocess (-j) support
 * 02/08/95 (seiwald) - -n implies -d2.
 * 02/22/95 (seiwald) - -v for version info.
 */

struct globs globs = {
	0,			/* noexec */
	1,			/* jobs */
	{ 0, 1 } 		/* debug ... */
} ;

/* Symbols to be defined as true for use in Jambase */

static char *othersyms[] = { OTHERSYMS, JAMVERSYM, 0 } ;
extern char **environ;

main( argc, argv )
char	**argv;
{
	int		n;
	char		*s;
	struct option	optv[N_OPTS];
	char		*all = "all";
	int		anyhow = 0;

	argc--, argv++;

	if( ( n = getoptions( argc, argv, "d:j:f:s:t:anv", optv ) ) < 0 )
	{
	    printf( "\nusage: jam [ options ] targets...\n\n" );

            printf( "-a      Build all targets, even if they are current.\n" );
            printf( "-dx     Set the debug level to x (0-9).\n" );
            printf( "-fx     Read x instead of Jambase.\n" );
            printf( "-jx     Run up to x shell commands concurrently.\n" );
            printf( "-n      Don't actually execute the updating actions.\n" );
	    printf( "-sx=y   Set variable x=y, overriding environment.\n" );
            printf( "-tx     Rebuild x, even if it is up-to-date.\n" );
            printf( "-v      Print the version of jam and exit.\n\n" );

	    exit( EXITBAD );
	}

	argc -= n, argv += n;

	/* Version info. */

	if( ( s = getoptval( optv, 'v', 0 ) ) )
	{
	    printf( "Jam - make(1) redux.  " );
	    printf( "Version %s.%s.  ", VERSION, PATCHLEVEL );
	    printf( "Copyright 1993, 1996 Christopher Seiwald.\n" );

	    return EXITOK;
	}

	/* Pick up interesting options */

	if( ( s = getoptval( optv, 'n', 0 ) ) )
	    globs.noexec++, globs.debug[2] = 1;

	if( ( s = getoptval( optv, 'a', 0 ) ) )
	    anyhow++;

	if( ( s = getoptval( optv, 'j', 0 ) ) )
	    globs.jobs = atoi( s );

	/* Turn on/off debugging */

	for( n = 0; s = getoptval( optv, 'd', n ); n++ )
	{
	    int i = atoi( s );
	    int j;

	    if( i < 0 || i >= DEBUG_MAX )
	    {
		printf( "Invalid debug level '%s'.\n" );
		continue;
	    }

	    /* +n turns on level n */
	    /* Just a number turns on levels 1..n, off n+1..max */

	    if( *s == '+' )
		globs.debug[i] = 1;
	    else for( j = 0; j < DEBUG_MAX; j++ )
		globs.debug[j] = j <= i;
	}

	/* load up environment variables */

	var_defines( othersyms );
	var_defines( environ );

	/* Load up variables set on command line. */

	for( n = 0; s = getoptval( optv, 's', n ); n++ )
	{
	    char *symv[2];
	    symv[0] = s;
	    symv[1] = 0;
	    var_defines( symv );
	}

	/* Parse ruleset */

	for( n = 0; s = getoptval( optv, 'f', n ); n++ )
	{
	    yyfparse( s );
	    yyparse();
	}

	if( !n )
	{
	    yyfparse( JAMBASE );
	    yyparse();
	}

	/* Manually touch -t targets */

	for( n = 0; s = getoptval( optv, 't', n ); n++ )
	    touchtarget( s );

	/* Now make target */

	if( argc )
	    make( argc, argv, anyhow );
	else
	    make( 1, &all, anyhow );

	/* Widely scattered cleanup */

	var_done();
	donerules();
	donestamps();
	donestr();

	return EXITOK;
}
