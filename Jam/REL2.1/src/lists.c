/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

# include "jam.h"
# include "newstr.h"
# include "lists.h"

/*
 * lists.c - maintain lists of strings
 *
 * The whole of jam relies on lists of strings as a datatype.  This
 * module, in conjunction with newstr.c, handles these relatively
 * efficiently.
 *
 * External routines:
 *
 *	list_append() - append a list onto another one, returning total
 *	list_new() - tack a string onto the end of a list of strings
 * 	list_copy() - copy a whole list of strings
 *	list_sublist() - copy a subset of a list of strings
 *	list_free() - free a list of strings
 *	list_print() - print a list of strings to stdout
 *
 * This implementation essentially uses a singly linked list, but
 * guarantees that the head element of every list has a valid pointer
 * to the tail of the list, so the new elements can efficiently and 
 * properly be appended to the end of a list.
 *
 * To avoid massive allocation, list_free() just tacks the whole freed
 * chain onto freelist and list_new() looks on freelist first for an 
 * available list struct.  list_free() does not free the strings in the 
 * chain: it lazily lets list_new() do so.
 *
 * 08/23/94 (seiwald) - new list_append()
 */

static LIST *freelist = 0;	/* junkpile for list_free() */

/*
 * list_append() - append a list onto another one, returning total
 */

LIST *
list_append( l, nl )
LIST	*l;
LIST	*nl;
{
	if( !nl )
	{
	    /* Just return l */
	}
	else if( !l )
	{
	    l = nl;
	}
	else
	{
	    /* Graft two non-empty lists. */
	    l->tail->next = nl;
	    l->tail = nl->tail;
	}

	return l;
}

/*
 * list_new() - tack a string onto the end of a list of strings
 */

LIST *
list_new( head, string )
LIST	*head;
char	*string;
{
	LIST *l;

	if( DEBUG_LISTS )
	    printf( "list > %s <\n", string );

	/* Get list struct from freelist, if one available.  */
	/* Otherwise allocate. */
	/* If from freelist, must free string first */

	if( freelist )
	{
	    l = freelist;
	    freestr( l->string );
	    freelist = freelist->next;
	}
	else
	{
	    l = (LIST *)malloc( sizeof( *l ) );
	}

	/* If first on chain, head points here. */
	/* If adding to chain, tack us on. */
	/* Tail must point to this new, last element. */

	if( !head ) head = l;
	else head->tail->next = l;
	head->tail = l;
	l->next = 0;

	l->string = string;

	return head;
}

/*
 * list_copy() - copy a whole list of strings (nl) onto end of another (l)
 */

LIST *
list_copy( l, nl )
LIST	*l;
LIST 	*nl;
{
	for( ; nl; nl = list_next( nl ) )
	    l = list_new( l, copystr( nl->string ) );

	return l;
}

/*
 * list_sublist() - copy a subset of a list of strings
 */

LIST *
list_sublist( l, start, count )
LIST	*l;
{
	LIST	*nl = 0;

	for( ; l && start--; l = list_next( l ) )
	    ;

	for( ; l && count--; l = list_next( l ) )
	    nl = list_new( nl, copystr( l->string ) );

	return nl;
}

/*
 * list_free() - free a list of strings
 */

void
list_free( head )
LIST	*head;
{
	/* Just tack onto freelist. */

	if( head )
	{
	    head->tail->next = freelist;
	    freelist = head;
	}
}

/*
 * list_print() - print a list of strings to stdout
 */

void
list_print( l )
LIST	*l;
{
	for( ; l; l = list_next( l ) )
	    printf( "%s ", l->string );
}
