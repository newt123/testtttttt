/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*
 * jam.h - includes and globals for jam
 *
 * 04/08/94 (seiwald) - Coherent/386 support added.
 * 04/21/94 (seiwald) - DGUX is __DGUX__, not just __DGUX.
 * 05/04/94 (seiwald) - new globs.jobs (-j jobs)
 * 11/01/94 (wingerd) - let us define path of Jambase at compile time.
 * 12/30/94 (wingerd) - changed command buffer size for NT (MS-DOS shell).
 * 02/22/95 (seiwald) - Jambase now in /usr/local/lib.
 * 04/30/95 (seiwald) - FreeBSD added.  Live Free or Die.
 * 05/10/95 (seiwald) - SPLITPATH character set up here.
 * 08/20/95 (seiwald) - added LINUX.
 * 08/21/95 (seiwald) - added NCR.
 * 10/23/95 (seiwald) - added SCO.
 */

# ifdef VMS

int unlink( char *f ); 	/* In filevms.c */

# include <types.h>
# include <file.h>
# include <stat.h>
# include <stdio.h>
# include <ctype.h>
# include <stdlib.h>
# include <signal.h>
# include <string.h>
# include <time.h>
# include <unixlib.h>

# ifdef __DECC
# define OTHERSYMS "VMS=true","OS=OPENVMS"
# else
# define OTHERSYMS "VMS=true","OS=VMS"
# endif 

# define MAXCMD	255 /* longest command - related to quotas */
# define JAMBASE "Jambase"
# define SPLITPATH ','
# define EXITOK 1
# define EXITBAD 0

# else

# ifdef NT

# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <ctype.h>
# include <malloc.h>
# include <memory.h>
# include <signal.h>
# include <string.h>
# include <time.h>

# define OTHERSYMS "NT=true","OS=NT"
# define JAMBASE "Jambase"
# define SPLITPATH ';'
# define MAXCMD	996	/* longest command */
# define EXITOK 0
# define EXITBAD 1

# else

# include <sys/types.h>
# include <sys/file.h>
# include <sys/stat.h>
# include <fcntl.h>
# ifndef ultrix
# include <stdlib.h>
# endif
# include <stdio.h>
# include <ctype.h>
# if !defined(__bsdi__)&&!defined(__FreeBSD__)&&!defined(NeXT)
# include <malloc.h>
# endif
# include <memory.h>
# include <signal.h>
# include <string.h>
# include <time.h>

# ifdef _AIX
# define unix
# define OTHERSYMS "UNIX=true","OS=AIX"
# endif
# ifdef __bsdi__
# define OTHERSYMS "UNIX=true","OS=BSDI"
# endif
# ifdef __DGUX__
# define OTHERSYMS "UNIX=true","OS=DGUX"
# endif
# ifdef __hpux
# define OTHERSYMS "UNIX=true","OS=HPUX"
# endif
# ifdef __osf__
# define OTHERSYMS "UNIX=true","OS=OSF"
# endif
# ifdef M_XENIX
# define OTHERSYMS "UNIX=true","OS=SCO"
# endif
# ifdef _SEQUENT_
# define OTHERSYMS "UNIX=true","OS=PTX"
# endif
# ifdef __sgi
# define OTHERSYMS "UNIX=true","OS=IRIX"
# endif
# ifdef sun
# define OTHERSYMS "UNIX=true","OS=SUNOS"
# endif
# ifdef solaris
# undef OTHERSYMS
# define OTHERSYMS "UNIX=true","OS=SOLARIS"
# endif
# ifdef ultrix
# define OTHERSYMS "UNIX=true","OS=ULTRIX"
# endif
# if defined (COHERENT) && defined (_I386)
# define OTHERSYMS "UNIX=true","OS=COHERENT/386"
# endif
# ifdef __FreeBSD__
# define OTHERSYMS "UNIX=true","OS=FreeBSD"
# endif
# ifdef linux
# define OTHERSYMS "UNIX=true","OS=LINUX"
# endif
# ifdef _ATT4
# define OTHERSYMS "UNIX=true","OS=NCR"
# endif
# ifndef OTHERSYMS
# define OTHERSYMS "UNIX=true"
# endif

# ifndef JAMBASE
# define JAMBASE "/usr/local/lib/Jambase"
# endif
# define MAXCMD	10240	/* longest command */
# define SPLITPATH ':'
# define EXITOK 0
# define EXITBAD 1

# endif /* NT */

# endif /* UNIX */

/* You probably don't need to muck with these. */

# define MAXSYM	1024	/* longest symbol in the environment */
# define MAXPATH 1024	/* longest filename */

# define MAXJOBS 64	/* silently enforce -j limit */
# define MAXARGC 32	/* words in $(JAMSHELL) */

/* Jam private definitions below. */

# define DEBUG_MAX	10

struct globs {
	int	noexec;
	int	jobs;
	char	debug[DEBUG_MAX];
} ;

extern struct globs globs;

# define DEBUG_MAKE	( globs.debug[ 1 ] )	/* show actions when executed */
# define DEBUG_MAKEQ	( globs.debug[ 2 ] )	/* show even quiet actions */
# define DEBUG_EXEC	( globs.debug[ 2 ] )	/* show text of actons */
# define DEBUG_MAKEPROG	( globs.debug[ 3 ] )	/* show progress of make0 */
# define DEBUG_EXECCMD	( globs.debug[ 3 ] )	/* show execcmds()'s work */

# define DEBUG_BIND	( globs.debug[ 4 ] )	/* show when files bound */
# define DEBUG_COMPILE	( globs.debug[ 5 ] )	/* show rule invocations */
# define DEBUG_MEM	( globs.debug[ 5 ] )	/* show memory use */
# define DEBUG_HEADER	( globs.debug[ 6 ] )	/* show result of header scan */
# define DEBUG_BINDSCAN	( globs.debug[ 6 ] )	/* show result of dir scan */
# define DEBUG_SEARCH	( globs.debug[ 6 ] )	/* show attempts at binding */

# define DEBUG_IF	( globs.debug[ 7 ] )	/* show 'if' calculations */
# define DEBUG_VARSET	( globs.debug[ 7 ] )	/* show variable settings */
# define DEBUG_VARGET	( globs.debug[ 8 ] )	/* show variable fetches */
# define DEBUG_VAREXP	( globs.debug[ 8 ] )	/* show variable expansions */
# define DEBUG_LISTS	( globs.debug[ 9 ] )	/* show list manipulation */
# define DEBUG_SCAN	( globs.debug[ 9 ] )	/* show scanner tokens */

