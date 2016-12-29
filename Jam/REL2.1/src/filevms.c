/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

# ifdef VMS

/*
 * filevms.c - manipulate file names and scan directories on VMS
 *
 * External routines:
 *
 *	file_parse() - split a file name into dir/base/suffix/member
 *	file_build() - build a filename given dir/base/suffix/member
 *	file_dirscan() - scan a directory for files
 *	file_time() - get timestamp of file, if not done by file_dirscan()
 *	file_archscan() - scan an archive for files
 *
 * File_parse() and file_build() just manipuate a string and a structure;
 * they do not make system calls.
 *
 * File_dirscan() and file_archscan() call back a caller provided function
 * for each file found.  A flag to this callback function lets file_dirscan()
 * and file_archscan() indicate that a timestamp is being provided with the
 * file.   If file_dirscan() or file_archscan() do not provide the file's
 * timestamp, interested parties may later call file_time().
 *
 * WARNING!  This file contains voodoo logic, as black magic is 
 * necessary for wrangling with VMS file name.  Woe be to people
 * who mess with this code.
 *
 * 02/09/95 (seiwald) - bungled R=[xxx] - was using directory length!
 */

# include <rms.h>
# include <iodef.h>
# include <ssdef.h>
# include <string.h>
# include <stdlib.h>
# include <ctype.h>
# include <stdio.h>
# include <descrip.h>

#include <lbrdef.h>
#include <credef.h>
#include <mhddef.h>
#include <lhidef.h>
#include <lib$routines.h>
#include <starlet.h>

# include "jam.h"
# include "filesys.h"

/* Supply missing prototypes for lbr$-routines*/
int lbr$close();
int lbr$get_index();
int lbr$ini_control();
int lbr$open();
int lbr$set_module();

/*
 * unlink() - remove a file
 */

unlink( f )
char *f;
{
	remove( f );
}

/*
 * file_parse() - split a file name into dir/base/suffix/member
 */

void
file_parse( file, f )
char		*file;
FILENAME	*f;
{
	char *p; 
	char *end;
	
	memset( (char *)f, 0, sizeof( *f ) );

	/* Look for <grist> */

	if( file[0] == '<' && ( p = strchr( file, '>' ) ) )
	{
	    f->f_grist.ptr = file + 1;
	    f->f_grist.len = p - file - 1;
	    file = p + 1;
	}

	/* Look for dev:[dir] or dev: */

	if( ( p = strchr( file, ']' ) ) || ( p = strchr( file, ':' ) ) )
	{
	    f->f_dir.ptr = file;
	    f->f_dir.len = p + 1 - file;
	    file = p + 1;
	}

	end = file + strlen( file );

	/* Look for (member) */

	if( ( p = strchr( file, '(' ) ) && end[-1] == ')' )
	{
	    f->f_member.ptr = p + 1;
	    f->f_member.len = end - p - 2;
	    end = p;
	} 

	/* Look for .suffix */

	if( ( p = strchr( file, '.' ) ) && p < end )
	{
	    f->f_suffix.ptr = p;
	    f->f_suffix.len = end - p;
	    end = p;
	}

	/* Leaves base */

	f->f_base.ptr = file;
	f->f_base.len = end - file;
}

/*
 * file_flags() - find out what's in a directory name
 *
 * VMS directories get complicated.  Valid combinations of root
 * and dir are:
 *
 *	root		dir		result
 *	----		---		------
 *					
 *	 		[dir]		[dir]
 *	dev				dev:
 *	dev		[dir]		dev:[dir]
 *	dev:				dev:
 *	dev:		[dir]		dev:[dir]
 *	[dir]				[dir]
 *	[dir]		[dir]		[dir.dir]
 *	dev:[dir]			dev:[dir]
 *	dev:[dir]	[dir]		dev:[dir.dir]
 *
 *	*		dev		dev
 *	*		dev:		dev:
 *	*		dev:[dir]	dev:[dir]
 *
 */

# define HAS_NOTHING	0
# define HAS_DEV	0x01
# define HAS_DIR	0x02
# define HAS_COLON	0x04

static int
file_flags( buf, len )
char	*buf;
int	len;
{
	int flags = 0;

	if( len && *buf != '[' )
	    flags |= HAS_DEV;

	while( len-- )
	    switch( *buf++ )
	{
	case ':':	flags |= HAS_COLON; break;
	case '[':	flags |= HAS_DIR; break;
	}

	return flags;
}

/*
 * file_build() - build a filename given dir/base/suffix/member
 */

void
file_build( f, file )
FILENAME	*f;
char		*file;
{

	int dir_flags = HAS_DEV;
	int root_flags = 0;

	if( f->f_grist.len )
	{
	    *file++ = '<';
	    memcpy( file, f->f_grist.ptr, f->f_grist.len );
	    file += f->f_grist.len;
	    *file++ = '>';
	}

	if( f->f_root.len )
	{
	    root_flags = file_flags( f->f_root.ptr, f->f_root.len );
	    dir_flags = file_flags( f->f_dir.ptr, f->f_dir.len );
	}

	switch( dir_flags & 0x03 )
	{
	case HAS_DIR:
	case HAS_NOTHING:
	    switch( root_flags & 0x03 )
	    {
	    case HAS_NOTHING:
		break;

	    case HAS_DEV:
		memcpy( file, f->f_root.ptr, f->f_root.len );
		file += f->f_root.len;
		if( !( root_flags & HAS_COLON ) )
		    *file++ = ':';
		break;
		
	    case HAS_DIR:
	    case HAS_DEV|HAS_DIR:
		memcpy( file, f->f_root.ptr, f->f_root.len );
		file += f->f_root.len;
		break;
	    }

	    if( dir_flags & HAS_DIR )
	    {
		if( root_flags & HAS_DIR )
		{
		    file[-1] = '.';
		    memcpy( file, f->f_dir.ptr + 1, f->f_dir.len - 1 );
		    file += f->f_dir.len - 1;
		}
		else
		{
		    memcpy( file, f->f_dir.ptr, f->f_dir.len );
		    file += f->f_dir.len;
		}
	    }

	    break;

	case HAS_DEV:
	case HAS_DEV|HAS_DIR:
	    memcpy( file, f->f_dir.ptr, f->f_dir.len );
	    file += f->f_dir.len;
	    break;
	}

	if( f->f_base.len )
	{
	    memcpy( file, f->f_base.ptr, f->f_base.len );
	    file += f->f_base.len;
	}

	if( f->f_suffix.len )
	{
	    memcpy( file, f->f_suffix.ptr, f->f_suffix.len );
	    file += f->f_suffix.len;
	}

	if( f->f_member.len )
	{
	    *file++ = '(';
	    memcpy( file, f->f_member.ptr, f->f_member.len );
	    file += f->f_member.len;
	    *file++ = ')';
	}
	*file = 0;
}

static void
file_cvttime( curtime, unixtime )
unsigned int *curtime;
time_t *unixtime;
{
    static const size_t divisor = 10000000;
    static unsigned int bastim[2] = { 0x4BEB4000, 0x007C9567 }; /* 1/1/1970 */
    int delta[2], remainder;

    lib$subx( curtime, bastim, delta );
    lib$ediv( &divisor, delta, unixtime, &remainder );
}

# define DEFAULT_FILE_SPECIFICATION "[]*.*;0"

# define min( a,b ) ((a)<(b)?(a):(b))

void
file_dirscan( char *dir, void (*func)() )
{

    struct FAB xfab;
    struct NAM xnam;
    struct XABDAT xab;
    char esa[256];
    char filename[256];
    char filename2[256];
    register status;
    FILENAME f;

    memset( (char *)&f, '\0', sizeof( f ) );

    f.f_dir.ptr = dir;
    f.f_dir.len = strlen( dir );

	/* get the input file specification
	 */
    xnam = cc$rms_nam;
    xnam.nam$l_esa = esa;
    xnam.nam$b_ess = sizeof( esa ) - 1;
    xnam.nam$l_rsa = filename;
    xnam.nam$b_rss = min( sizeof( filename ) - 1, NAM$C_MAXRSS );

    xab = cc$rms_xabdat;                /* initialize extended attributes */
    xab.xab$b_cod = XAB$C_DAT;		/* ask for date */
    xab.xab$l_nxt = NULL;               /* terminate XAB chain      */

    xfab = cc$rms_fab;
    xfab.fab$l_dna = DEFAULT_FILE_SPECIFICATION;
    xfab.fab$b_dns = sizeof( DEFAULT_FILE_SPECIFICATION ) - 1;
    xfab.fab$l_fop = FAB$M_NAM;
    xfab.fab$l_fna = dir;			/* address of file name	    */
    xfab.fab$b_fns = strlen( dir );		/* length of file name	    */
    xfab.fab$l_nam = &xnam;			/* address of NAB block	    */
    xfab.fab$l_xab = (char *)&xab;       /* address of XAB block     */


    status = sys$parse( &xfab );

    if( DEBUG_BINDSCAN )
	printf( "scan directory %s\n", dir );

    if ( !( status & 1 ) )
	return;

    while ( (status = sys$search( &xfab )) & 1 )
    {
	char *s;
	time_t time;

	/* "I think that might work" - eml */

	sys$open( &xfab );
	sys$close( &xfab );

	filename[xnam.nam$b_rsl] = '\0';
	for( s = xnam.nam$l_name; *s; s++ )
	    if( isupper( *s ) )
		*s = tolower( *s );

	f.f_base.ptr = xnam.nam$l_name;
	f.f_base.len = xnam.nam$b_name;
	f.f_suffix.ptr = xnam.nam$l_type;
	f.f_suffix.len = xnam.nam$b_type;

	file_build( &f, filename2 );

	file_cvttime( &xab.xab$q_rdt, &time );

	(*func)( filename2, 1 /* time valid */, time );
    }

    if ( status != RMS$_NMF && status != RMS$_FNF )
	lib$signal( xfab.fab$l_sts, xfab.fab$l_stv );
}    

int
file_time( filename, time )
char	*filename;
time_t	*time;
{
	/* This should never be called, as all files are */
	/* timestampped in file_dirscan() and file_archscan() */
	return -1;
}

static char *VMS_archive = 0;
static void (*VMS_func)() = 0;
static void *context;

static int
file_archmember( module, rfa )
struct dsc$descriptor_s *module;
unsigned long *rfa;
{
    static struct dsc$descriptor_s bufdsc =
		  {0, DSC$K_DTYPE_T, DSC$K_CLASS_S, NULL};

    struct mhddef *mhd;
    char filename[128];
    char buf[ MAXPATH ];

    int library_date, status;

    register int i;
    register char *p;

    bufdsc.dsc$a_pointer = filename;
    bufdsc.dsc$w_length = sizeof( filename );
    status = lbr$set_module( &context, rfa, &bufdsc,
			     &bufdsc.dsc$w_length, NULL );
    if ( !(status & 1) )
	return ( 1 );

    mhd = (struct mhddef *)filename;

    file_cvttime( &mhd->mhd$l_datim, &library_date );

    for ( i = 0, p = module->dsc$a_pointer; i < module->dsc$w_length; i++, p++ )
	filename[i] = _tolower( *p );

    filename[i] = '\0';

    sprintf( buf, "%s(%s.obj)", VMS_archive, filename );

    (*VMS_func)( buf, 1 /* time valid */, (time_t)library_date );

    return ( 1 );
}

void
file_archscan( archive, func )
char *archive;
void (*func)();
{
    static struct dsc$descriptor_s library =
		  {0, DSC$K_DTYPE_T, DSC$K_CLASS_S, NULL};

    unsigned long lfunc = LBR$C_READ;
    unsigned long typ = LBR$C_TYP_UNK;
    unsigned long index = 1;

    register status;

    VMS_archive = archive;
    VMS_func = func;

    status = lbr$ini_control( &context, &lfunc, &typ, NULL );
    if ( !( status & 1 ) )
	return;

    library.dsc$a_pointer = archive;
    library.dsc$w_length = strlen( archive );

    status = lbr$open( &context, &library, NULL, NULL, NULL, NULL, NULL );
    if ( !( status & 1 ) )
	return;

    (void) lbr$get_index( &context, &index, file_archmember, NULL );

    (void) lbr$close( &context );
}

# endif /* VMS */

