/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

# include "jam.h"
# include "lists.h"
# include "parse.h"
# include "scan.h"
# include "jamgram.h"
# include "newstr.h"

/*
 * scan.c - the jam yacc scanner
 *
 * 12/26/93 (seiwald) - bump buf in yylex to 10240 - yuk.
 * 09/16/94 (seiwald) - check for overflows, unmatched {}'s, etc.
 *			Also handle tokens abutting EOF by remembering
 *			to return EOF now matter how many times yylex()
 *			reinvokes yyline().
 * 02/11/95 (seiwald) - honor only punctuation keywords if SCAN_PUNCT.
 * 07/27/95 (seiwald) - Include jamgram.h after scan.h, so that YYSTYPE is
 *			defined before Linux's yacc tries to redefine it.
 */

struct keyword {
	char *word;
	int type;
} keywords[] = {
#ifdef FATFS
# include "jamgramt.h"
#else
# include "jamgramtab.h"
#endif
	0, 0
} ;

# define MAX_INCLUDES 10

static struct {
	char *string;
	FILE *file;
	char *fname;
	int line;
	char buf[ 512 ];
} includes[ MAX_INCLUDES ] = {0}, *incp = includes;

static int incdepth = 0;

static int scanmode = SCAN_NORMAL;

static char *symdump();

/* 
 */

void
yymode( n )
int n;
{
	scanmode = n;
}

yyerror( s )
char *s;
{
	if( incdepth )
	    printf( "%s: line %d: ", incp->fname, incp->line );

	printf( "%s at %s\n", s, symdump( &yylval ) );
}

void
yyfparse( s )
char *s;
{
	FILE *f = stdin;

	if( incdepth == MAX_INCLUDES )
	{
	    printf( "%s: too many levels of nested includes\n", s );
	    return;
	}

	if( strcmp( s, "-" ) && !( f = fopen( s, "r" ) ) )
	    perror( s );

	incp = &includes[ incdepth++ ];
	incp->string = "";
	incp->file = f;
	incp->fname = s;
	incp->line = 0;
}

/*
 * get character routine
 */

yyline()
{
top:
	if( incp->file )
	{
	    if( fgets( incp->buf, sizeof( incp->buf ), incp->file ) )
	    {
		incp->line++;
		incp->string = incp->buf;
		return *incp->string++;
	    }

	    if( incp->file != stdin )
		    fclose( incp->file );
	} 

	/* Last include file? */

	if( incdepth <= 1  )
	{
	    /* Make sure subsequent calls get EOF, too */

	    incdepth = 0;
	    incp->string = "";

	    return EOF;
	}

	/* Pop to previous include */

	incp = &includes[ --incdepth - 1 ];

	if( !*incp->string )
	    goto top;

	return *incp->string++;
}

/*
 * yylex() - set yylval to current token; return its type
 */

# define yychar() ( *incp->string ? *incp->string++ : yyline() )

yylex()
{
	int c = *incp->string;
	char buf[10240];

	for( ;; )
	{
		/* Skip past the "" that yyfparse() points incp->string at. */

		if( !c )
		    c = yychar();

		/* Skip past white space */

		while( c != EOF && isspace( c ) )
			c = yychar();

		/* Not a comment?  Swallow up comment line. */

		if( c != '#' )
			break;
		while( ( c = yychar() ) != EOF && c != '\n' )
			;
	}

	/* c now points to the first character of a token. */

	if( c == EOF )
	{
	    goto eof;
	} 
	else if( c == '{' && scanmode == SCAN_STRING )
	{
		/* look for closing { */

		char *b = buf;
		int nest = 1;

		while( ( c = yychar() ) != EOF && b < buf + sizeof( buf ) )
		{
			if( c == '{' )
				nest++;
			else if( c == '}' )
				nest--;
			if( !nest )
			    break;
			*b++ = c;
		}

		/* Check obvious errors. */

		if( b == buf + sizeof( buf ) )
		{
		    yyerror( "action block too big" );
		    goto eof;
		}

		if( nest )
		{
		    yyerror( "unmatched {} in action block" );
		    goto eof;
		}

		*b = 0;
		yylval.type = STRING;
		yylval.string = newstr( buf );
	}
	else 
	{
		/* look for white space to delimit word */
		/* "'s get stripped but preserve white space */

		char *b = buf;
		int inquote = 0;
		int literal = 0;
		int hasquote = 0;
		struct keyword *k;

		while( b < buf + sizeof( buf ) )
		{
		    if( literal )
			*b++ = c, literal = 0;
		    else if( c == '\\' )
			literal++;
		    else if( c == '"' )
			inquote = !inquote, hasquote++;
		    else
			*b++ = c;

		    if( ( c = yychar() ) == EOF || !inquote && isspace( c ) )
			break;
		}

		/* Check obvious errors. */

		if( b == buf + sizeof( buf ) )
		{
		    yyerror( "string too big" );
		    goto eof;
		}

		if( inquote )
		{
		    yyerror( "unmatched \" in string" );
		    goto eof;
		}

		/* We looked ahead a character - back up. */

		incp->string--;

		/* scan token table */
		/* don't scan if it's "anything", $anything, */
		/* or an alphabetic when were looking for punctuation */

		*b = 0;
		yylval.type = ARG;

		if( !hasquote && 
		    *buf != '$' && 
		    !( isalpha( *buf ) && scanmode == SCAN_PUNCT ) )
		{
		    for( k = keywords; k->word; k++ )
			if( *buf == *k->word && !strcmp( k->word, buf ) )
		    {
			yylval.type = k->type;
			yylval.string = k->word;	/* used by symdump */
			break;
		    }
		}

		if( yylval.type == ARG )
		    yylval.string = newstr( buf );
	}

	if( DEBUG_SCAN )
		printf( "scan %s\n", symdump( &yylval ) );

	return yylval.type;

eof:
	yylval.type = EOF;
	return yylval.type;
}

static char *
symdump( s )
YYSTYPE *s;
{
	static char buf[ 512 ];

	switch( s->type )
	{
	case EOF:
		sprintf( buf, "EOF" );
		break;
	case 0:
		sprintf( buf, "unknown symbol %s", s->string );
		break;
	case ARG:
		sprintf( buf, "argument %s", s->string );
		break;
	case STRING:
		sprintf( buf, "string \"%s\"", s->string );
		break;
	default:
		sprintf( buf, "keyword %s", s->string );
		break;
	}
	return buf;
}
