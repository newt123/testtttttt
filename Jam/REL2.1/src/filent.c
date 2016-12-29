/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

# ifdef NT

# include "jam.h"
# include "filesys.h"

# include <io.h>

/*
 * fileunix.c - manipulate file names and scan directories on UNIX
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
 * 12/21/94 (wingerd) Use backslashes for pathnames - the NT way.
 * 02/23/95 (wingerd) Compilers on NT can handle "/" in pathnames, so we
 *                    should expect hdr searches to come up with strings
 *                    like "thing/thing.h". So we need to test for "/" as
 *                    well as "\" when parsing pathnames.
 * 02/14/95 (seiwald) - parse and build /xxx properly
 * 03/16/95 (seiwald) - fixed accursed typo on line 69.
 * 07/10/95 (taylor)  Findfirst() returns the first file on NT.
 */

/*
 * file_parse() - split a file name into dir/base/suffix/member
 */

void
file_parse( file, f )
char		*file;
FILENAME	*f;
{
	char *p, *p1; 
	char *end;
	
	memset( (char *)f, 0, sizeof( *f ) );

	/* Look for <grist> */

	if( file[0] == '<' && ( p = strchr( file, '>' ) ) )
	{
	    f->f_grist.ptr = file + 1;
	    f->f_grist.len = p - file - 1;
	    file = p + 1;
	}

	/* Look for dir/ */

	p = strrchr( file, '\\' );
	p1 = strrchr( file, '/' );
	p = p1 > p ? p1 : p;

 	/* Special case for \ - dirname is \, not "" */

	if( p )
	{
	    f->f_dir.ptr = file;
 	    f->f_dir.len = p == file ? 1 : p - file;
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
 * file_build() - build a filename given dir/base/suffix/member
 */

void
file_build( f, file )
FILENAME	*f;
char		*file;
{
	if( f->f_grist.len )
	{
	    *file++ = '<';
	    memcpy( file, f->f_grist.ptr, f->f_grist.len );
	    file += f->f_grist.len;
	    *file++ = '>';
	}

	/* Don't prepend root if it's . or directory is rooted */

	if( f->f_root.len 
	    && !( f->f_root.len == 1 && f->f_root.ptr[0] == '.' )
	    && !( f->f_dir.len && f->f_dir.ptr[0] == '\\' )
	    && !( f->f_dir.len && f->f_dir.ptr[0] == '/' )
	    && !( f->f_dir.len && f->f_dir.ptr[1] == ':' ) )
	{
	    memcpy( file, f->f_root.ptr, f->f_root.len );
	    file += f->f_root.len;
	    *file++ = '\\';
	}
	    
	if( f->f_dir.len )
	{
	    memcpy( file, f->f_dir.ptr, f->f_dir.len );
	    file += f->f_dir.len;
	}

	/* Put \ between dir and file */

	if( f->f_dir.len && ( f->f_base.len || f->f_suffix.len ) )
	{
	    /* Special case for dir \ : don't add another \ */

	    if( !( f->f_dir.len == 1 && f->f_dir.ptr[0] == '\\' ) )
		*file++ = '\\';
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

/*
 * file_dirscan() - scan a directory for files
 */

void
file_dirscan( dir, func )
char	*dir;
void	(*func)();
{
	FILENAME f;
	char filespec[ MAXPATH ];
	char filename[ MAXPATH ];
	long handle;
	int ret;
	struct _finddata_t fileinfo[1];

	/* First enter directory itself */

	memset( (char *)&f, '\0', sizeof( f ) );

	f.f_dir.ptr = dir;
	f.f_dir.len = strlen(dir);

	dir = *dir ? dir : ".";

 	/* Special case \ : enter it */
 
 	if( f.f_dir.len == 1 && f.f_dir.ptr[0] == '\\' )
 	    (*func)( dir, 0 /* not stat()'ed */, (time_t)0 );

	/* Now enter contents of directory */

	sprintf( filespec, "%s/*", dir );

	if( DEBUG_BINDSCAN )
	    printf( "scan directory %s\n", dir );

	handle = _findfirst( filespec, fileinfo );

	if( ret = ( handle < 0L ) )
	    return;

	while( !ret )
	{
	    f.f_base.ptr = fileinfo->name;
	    f.f_base.len = strlen( fileinfo->name );

	    file_build( &f, filename );

	    (*func)( filename, 1 /* stat()'ed */, fileinfo->time_write );

		ret = _findnext( handle, fileinfo );
	}

	_findclose( handle );
}

/*
 * file_time() - get timestamp of file, if not done by file_dirscan()
 */

int
file_time( filename, time )
char	*filename;
time_t	*time;
{
	return 0;
}

/*
 * file_archscan() - scan an archive for files
 */

/* Straight from SunOS */

#define	ARMAG	"!<arch>\n"
#define	SARMAG	8

#define	ARFMAG	"`\n"

struct ar_hdr {
	char	ar_name[16];
	char	ar_date[12];
	char	ar_uid[6];
	char	ar_gid[6];
	char	ar_mode[8];
	char	ar_size[10];
	char	ar_fmag[2];
};

# define SARFMAG 2
# define SARHDR sizeof( struct ar_hdr )

void
file_archscan( archive, func )
char *archive;
void (*func)();
{
	struct ar_hdr ar_hdr;
	char *string_table = 0;
	char buf[ MAXPATH ];
	long offset;
	int fd;

	if( ( fd = open( archive, O_RDONLY, 0 ) ) < 0 )
	    return;

	if( read( fd, buf, SARMAG ) != SARMAG ||
	    strncmp( ARMAG, buf, SARMAG ) )
	{
	    close( fd );
	    return;
	}

	offset = SARMAG;

	if( DEBUG_BINDSCAN )
	    printf( "scan archive %s\n", archive );

	while( read( fd, &ar_hdr, SARHDR ) == SARHDR &&
	       !memcmp( ar_hdr.ar_fmag, ARFMAG, SARFMAG ) )
	{
	    long    lar_date;
	    long    lar_size;
	    char    *name = 0;
 	    char    *endname;
	    char    *c;

	    sscanf( ar_hdr.ar_date, "%ld", &lar_date );
	    sscanf( ar_hdr.ar_size, "%ld", &lar_size );

	    lar_size = ( lar_size + 1 ) & ~1;

	    if (ar_hdr.ar_name[0] == '/' && ar_hdr.ar_name[1] == '/' )
	    {
		/* this is the "string table" entry of the symbol table,
		** which holds strings of filenames that are longer than
		** 15 characters (ie. don't fit into a ar_name
		*/

		string_table = malloc(lar_size);
		if (read(fd, string_table, lar_size) != lar_size)
		    printf("error reading string table\n");
		offset += SARHDR + lar_size;
		continue;
	    }
	    else if (ar_hdr.ar_name[0] == '/' && ar_hdr.ar_name[1] != ' ')
	    {
		/* Long filenames are recognized by "/nnnn" where nnnn is
		** the offset of the string in the string table represented
		** in ASCII decimals.
		*/

		name = string_table + atoi( ar_hdr.ar_name + 1 );
		endname = name + strlen( name );
	    }
	    else
	    {
		/* normal name */
		name = ar_hdr.ar_name;
		endname = name + sizeof( ar_hdr.ar_name );
	    }

	    /* strip trailing space, slashes, and backslashes */

	    while( endname-- > name )
		if( *endname != ' ' && *endname != '\\' && *endname != '/' )
		    break;
	    *++endname = 0;

	    /* strip leading directory names, an NT specialty */

	    if( c = strrchr( name, '/' ) )
		name = c + 1;
	    if( c = strrchr( name, '\\' ) )
		name = c + 1;

	    sprintf( buf, "%s(%.*s)", archive, endname - name, name );
	    (*func)( buf, 1 /* time valid */, (time_t)lar_date );

	    offset += SARHDR + lar_size;
	    lseek( fd, offset, 0 );
	}

	close( fd );
}

# endif /* NT */
