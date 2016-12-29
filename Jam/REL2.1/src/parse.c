/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

# include "jam.h"
# include "lists.h"
# include "parse.h"
# include "newstr.h"

/*
 * parse.c - make and destroy parse trees as driven by the parser
 */

PARSE *
parse_make( func, left, right, string, string1, llist, rlist, num )
void	(*func)();
PARSE	*left;
PARSE	*right;
char	*string;
char	*string1;
LIST	*llist;
LIST	*rlist;
int	num;
{
	PARSE	*p = (PARSE *)malloc( sizeof( PARSE ) );

	p->func = func;
	p->left = left;
	p->right = right;
	p->string = string;
	p->string1 = string1;
	p->llist = llist;
	p->rlist = rlist;
	p->num = num;

	return p;
}

void
parse_free( p )
PARSE	*p;
{
	if( p->string )
	    freestr( p->string );
	if( p->string1 )
	    freestr( p->string1 );
	if( p->llist )
	    list_free( p->llist );
	if( p->rlist )
	    list_free( p->rlist );
	if( p->left )
	    parse_free( p->left );
	if( p->right )
	    parse_free( p->right );
	
	free( (char *)p );
}
