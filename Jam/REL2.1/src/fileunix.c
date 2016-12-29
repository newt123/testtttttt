/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

# include "jam.h"
# include "filesys.h"

# ifdef unix

# if defined(_SEQUENT_) || defined(__DGUX__) || defined(M_XENIX)
# define PORTAR 1
# endif

#ifdef NeXT
# include <sys/dir.h>
#else
# include <dirent.h>
#endif

# if defined (COHERENT) && defined (_I386)
# include <arcoff.h>
# else
# include <ar.h>
# endif	/* COHERENT */
  

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
 * 12/26/93 (seiwald) - handle dir/.suffix properly in file_build()
 * 04/08/94 (seiwald) - Coherent/386 support added.
 * 12/19/94 (mikem) - solaris string table insanity support
 * 02/14/95 (seiwald) - parse and build /xxx properly
 */

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

	/* Look for dir/ */
	/* Special case for / - dirname is /, not "" */

	if( p = strrchr( file, '/' ) )
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
	    && !( f->f_dir.len && f->f_dir.ptr[0] == '/' ) )
	{
	    memcpy( file, f->f_root.ptr, f->f_root.len );
	    file += f->f_root.len;
	    *file++ = '/';
	}
	    
	if( f->f_dir.len )
	{
	    memcpy( file, f->f_dir.ptr, f->f_dir.len );
	    file += f->f_dir.len;
	}

	/* Put / between dir and file */

	if( f->f_dir.len && ( f->f_base.len || f->f_suffix.len ) )
	{
	    /* Special case for dir / : don't add another / */

	    if( !( f->f_dir.len == 1 && f->f_dir.ptr[0] == '/' ) )
		*file++ = '/';
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
	DIR *d;
#ifdef NeXT
	struct direct *dirent;
#else
	struct dirent *dirent;
#endif
	char filename[ MAXPATH ];
	struct stat statbuf;

	/* First enter directory itself */

	memset( (char *)&f, '\0', sizeof( f ) );

	f.f_dir.ptr = dir;
	f.f_dir.len = strlen(dir);

	dir = *dir ? dir : ".";

	/* Special case / : enter it */

	if( f.f_dir.len == 1 && f.f_dir.ptr[0] == '/' )
	    (*func)( dir, 0 /* not stat()'ed */, (time_t)0 );

	/* Now enter contents of directory */

	if( !( d = opendir( dir ) ) )
	    return;

	if( DEBUG_BINDSCAN )
	    printf( "scan directory %s\n", dir );

	while( dirent = readdir( d ) )
	{
	    f.f_base.ptr = dirent->d_name;
	    f.f_base.len = strlen( dirent->d_name );

	    file_build( &f, filename );

	    (*func)( filename, 0 /* not stat()'ed */, (time_t)0 );
	}

	closedir( d );
}

/*
 * file_time() - get timestamp of file, if not done by file_dirscan()
 */

int
file_time( filename, time )
char	*filename;
time_t	*time;
{
	struct stat statbuf;

	if( stat( filename, &statbuf ) < 0 )
	    return -1;

	*time = statbuf.st_mtime;
	return 0;
}

/*
 * file_archscan() - scan an archive for files
 */

# ifndef AIAMAG	/* God-fearing UNIX */

# define SARFMAG 2
# define SARHDR sizeof( struct ar_hdr )

void
file_archscan( archive, func )
char *archive;
void (*func)();
{
	struct ar_hdr ar_hdr;
	char buf[ MAXPATH ];
	long offset;
	char    *string_table = 0;
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
	    char    lar_name[256];
	    long    lar_date;
	    long    lar_size;
	    long    lar_offset;
	    char *c;
	    char    *src, *dest;

	    strncpy( lar_name, ar_hdr.ar_name, sizeof(ar_hdr.ar_name) );

	    sscanf( ar_hdr.ar_date, "%ld", &lar_date );
	    sscanf( ar_hdr.ar_size, "%ld", &lar_size );

	    if (ar_hdr.ar_name[0] == '/')
	    {
		if (ar_hdr.ar_name[1] == '/')
		{
		    /* this is the "string table" entry of the symbol table,
		    ** which holds strings of filenames that are longer than
		    ** 15 characters (ie. don't fit into a ar_name
		    */

		    string_table = malloc(lar_size);
		    lseek(fd, offset + SARHDR, 0);
		    if (read(fd, string_table, lar_size) != lar_size)
			printf("error reading string table\n");
		}
		else if (ar_hdr.ar_name[1] != ' ')
		{
		    /* Long filenames are recognized by "/nnnn" where nnnn is
		    ** the offset of the string in the string table represented
		    ** in ASCII decimals.
		    */
		    dest = lar_name;
		    lar_offset = atoi(lar_name + 1);
		    src = &string_table[lar_offset];
		    while (*src != '/')
			*dest++ = *src++;
		    *dest = '/';
		}
	    }

	    c = lar_name - 1;
	    while( *++c != ' ' && *c != '/' )
		;
	    *c = '\0';

	    if ( DEBUG_BINDSCAN )
		printf( "archive name %s found\n", lar_name );

	    sprintf( buf, "%s(%s)", archive, lar_name );

	    (*func)( buf, 1 /* time valid */, (time_t)lar_date );

	    offset += SARHDR + ( ( lar_size + 1 ) & ~1 );
	    lseek( fd, offset, 0 );
	}

	if (string_table)
	    free(string_table);

	close( fd );
}

# else /* AIAMAG - RS6000 AIX */

void
file_archscan( archive, func )
char *archive;
void (*func)();
{
	struct fl_hdr fl_hdr;

	struct {
		struct ar_hdr hdr;
		char pad[ 256 ];
	} ar_hdr ;

	char buf[ MAXPATH ];
	long offset;
	int fd;

	if( ( fd = open( archive, O_RDONLY, 0 ) ) < 0 )
	    return;

	if( read( fd, (char *)&fl_hdr, FL_HSZ ) != FL_HSZ ||
	    strncmp( AIAMAG, fl_hdr.fl_magic, SAIAMAG ) )
	{
	    close( fd );
	    return;
	}

	sscanf( fl_hdr.fl_fstmoff, "%ld", &offset );

	if( DEBUG_BINDSCAN )
	    printf( "scan archive %s\n", archive );

	while( offset > 0 &&
	       lseek( fd, offset, 0 ) >= 0 &&
	       read( fd, &ar_hdr, sizeof( ar_hdr ) ) >= sizeof( ar_hdr.hdr ) )
	{
	    long    lar_date;
	    int	    lar_namlen;

	    sscanf( ar_hdr.hdr.ar_namlen, "%d", &lar_namlen );
	    sscanf( ar_hdr.hdr.ar_date, "%ld", &lar_date );
	    sscanf( ar_hdr.hdr.ar_nxtmem, "%ld", &offset );

	    if( !lar_namlen )
		continue;

	    ar_hdr.hdr._ar_name.ar_name[ lar_namlen ] = '\0';

	    sprintf( buf, "%s(%s)", archive, ar_hdr.hdr._ar_name.ar_name );

	    (*func)( buf, 1 /* time valid */, (time_t)lar_date );
	}

	close( fd );
}

# endif /* AIAMAG - RS6000 AIX */

# endif /* unix */

