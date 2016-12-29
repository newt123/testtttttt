/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

# include "jam.h"
# include "hash.h"
# include "filesys.h"
#ifdef FATFS
# include "timestam.h"
#else
# include "timestamp.h"
#endif
# include "newstr.h"

/*
 * timestamp.c - get the timestamp of a file or archive member
 */

/*
 * BINDING - all known files
 */

typedef struct _binding BINDING;

struct _binding {
	char	*name;
	short	flags;

# define BIND_SCANNED	0x01	/* if directory or arch, has been scanned */

	short	progress;

# define BIND_INIT	0	/* never seen */
# define BIND_NOENTRY	1	/* timestamp requested but file never found */
# define BIND_SPOTTED	2	/* file found but not timed yet */
# define BIND_MISSING	3	/* file found but can't get timestamp */
# define BIND_FOUND	4	/* file found and time stamped */

	time_t	time;		/* update time - 0 if not exist */
} ;

static struct hash *bindhash = 0;
static void time_enter();

static char *time_progress[] =
{
	"INIT",
	"NOENTRY",
	"SPOTTED",
	"MISSING",
	"FOUND"
} ;


/*
 * timestamp() - return timestamp on a file, if present
 */

void
timestamp( target, time )
char	*target;
time_t	*time;
{
	FILENAME f1, f2;
	BINDING	binding, *b = &binding;
	char buf[ MAXPATH ];

	if( !bindhash )
	    bindhash = hashinit( sizeof( BINDING ), "bindings" );

	/* Quick path - is it there? */

	b->name = target;
	b->time = b->flags = 0;
	b->progress = BIND_INIT;

	if( hashenter( bindhash, (HASHDATA **)&b ) )
	    b->name = newstr( target );		/* never freed */

	if( b->progress != BIND_INIT )
	    goto afterscanning;

	b->progress = BIND_NOENTRY;

	/* Not found - have to scan for it */

	file_parse( target, &f1 );
	memset( (char *)&f2, '\0', sizeof( f2 ) );

	/* Scan directory if not already done so */

	{
	    BINDING binding, *b = &binding;

	    f2.f_dir = f1.f_dir;
	    file_build( &f2, buf );

	    b->name = buf;
	    b->time = b->flags = 0;
	    b->progress = BIND_INIT;

	    if( hashenter( bindhash, (HASHDATA **)&b ) )
		b->name = newstr( buf );	/* never freed */

	    if( !( b->flags & BIND_SCANNED ) )
	    {
		file_dirscan( buf, time_enter );
		b->flags |= BIND_SCANNED;
	    }
	}

	/* Scan archive if not already done so */

	if( f1.f_member.len )
	{
	    BINDING binding, *b = &binding;

	    f2.f_base = f1.f_base;
	    f2.f_suffix = f1.f_suffix;
	    file_build( &f2, buf );

	    b->name = buf;
	    b->time = b->flags = 0;
	    b->progress = BIND_INIT;

	    if( hashenter( bindhash, (HASHDATA **)&b ) )
		b->name = newstr( buf );	/* never freed */

	    if( !( b->flags & BIND_SCANNED ) )
	    {
		file_archscan( buf, time_enter );
		b->flags |= BIND_SCANNED;
	    }
	}

    afterscanning:

	if( b->progress == BIND_SPOTTED )
	{
	    if( file_time( b->name, &b->time ) < 0 )
		b->progress = BIND_MISSING;
	    else
		b->progress = BIND_FOUND;
	}

	*time = b->progress == BIND_FOUND ? b->time : 0;

	if( DEBUG_BIND && b->progress == BIND_FOUND )
	{
	    printf( "time ( %s ) : %s", target, ctime( time ) );
	}
}

static void
time_enter( target, found, time )
char	*target;
int	found;
time_t	time;
{
	BINDING	binding, *b = &binding;

	b->name = target;
	b->flags = 0;

	if( hashenter( bindhash, (HASHDATA **)&b ) )
	    b->name = newstr( target );		/* never freed */

	b->time = time;
	b->progress = found ? BIND_FOUND : BIND_SPOTTED;

	if( DEBUG_BINDSCAN )
	    printf( "time ( %s ) : %s\n", target, time_progress[b->progress] );
}

/*
 * donestamps() - free timestamp tables
 */

void
donestamps()
{
	hashdone( bindhash );
}
