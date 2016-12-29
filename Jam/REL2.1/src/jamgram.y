%token _BANG
%token _BANG_EQUALS
%token _AMPERAMPER
%token _LPAREN
%token _RPAREN
%token _PLUS_EQUALS
%token _COLON
%token _SEMIC
%token _LANGLE
%token _LANGLE_EQUALS
%token _EQUALS
%token _RANGLE
%token _RANGLE_EQUALS
%token _QUESTION_EQUALS
%token ACTIONS
%token CASE
%token DEFAULT
%token ELSE
%token EXISTING
%token FOR
%token IF
%token IGNORE
%token IN
%token INCLUDE
%token ON
%token PIECEMEAL
%token QUIETLY
%token RULE
%token SWITCH
%token TOGETHER
%token UPDATED
%token _LBRACE
%token _BARBAR
%token _RBRACE
/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*
 * jamgram.yy - jam grammar
 *
 * 04/13/94 (seiwald) - added shorthand L0 for null list pointer
 * 06/01/94 (seiwald) - new 'actions existing' does existing sources
 * 08/23/94 (seiwald) - Support for '+=' (append to variable)
 * 08/31/94 (seiwald) - Allow ?= as alias for "default =".
 * 09/15/94 (seiwald) - if conditionals take only single arguments, so
 *			that 'if foo == bar' gives syntax error (use =).
 * 02/11/95 (seiwald) - when scanning arguments to rules, only treat
 *			punctuation keywords as keywords.  All arg lists
 *			are terminated with punctuation keywords.
 */

%token ARG STRING

%left _BARBAR
%left _AMPERAMPER
%left _BANG

%{
#include "jam.h"

#include "lists.h"
#include "parse.h"
#include "scan.h"
#include "compile.h"
#include "newstr.h"

# define F0 (void (*)())0
# define P0 (PARSE *)0
# define S0 (char *)0

# define pset( l,r,a ) 	  parse_make( compile_set,P0,P0,S0,S0,l,r,a )
# define pset1( l,p,a )	  parse_make( compile_settings,p,P0,S0,S0,l,L0,a )
# define pstng( p,l,r,a ) pset1( p, parse_make( F0,P0,P0,S0,S0,l,r,0 ), a )
# define prule( s,l,r )   parse_make( compile_rule,P0,P0,s,S0,l,r,0 )
# define prules( l,r )	  parse_make( compile_rules,l,r,S0,S0,L0,L0,0 )
# define pfor( s,p,l )    parse_make( compile_foreach,p,P0,s,S0,l,L0,0 )
# define psetc( s,p )     parse_make( compile_setcomp,p,P0,s,S0,L0,L0,0 )
# define psete( s,s1,f )  parse_make( compile_setexec,P0,P0,s,s1,L0,L0,f )
# define pincl( l )       parse_make( compile_include,P0,P0,S0,S0,l,L0,0 )
# define pswitch( l,p )   parse_make( compile_switch,p,P0,S0,S0,l,L0,0 )
# define pcases( l,r )    parse_make( F0,l,r,S0,S0,L0,L0,0 )
# define pcase( s,p )     parse_make( F0,p,P0,s,S0,L0,L0,0 )
# define pif( l,r )	  parse_make( compile_if,l,r,S0,S0,L0,L0,0 )
# define pthen( l,r )	  parse_make( F0,l,r,S0,S0,L0,L0,0 )
# define pcond( c,l,r )	  parse_make( F0,l,r,S0,S0,L0,L0,c )
# define pcomp( c,l,r )	  parse_make( F0,P0,P0,S0,S0,l,r,c )

%}

%%

/*
 * stmts - the contents of a JAMFILE
 */

stmts	: 
		{
			compile_builtins();
		}
	| stmts rule
		{ 
			(*($2.parse->func))( $2.parse, L0, L0 );
			parse_free( $2.parse );
		}
	;

/*
 * rules - a strings of rule's together
 * rule - any one of jam's rules
 */

rules	: /* empty */
		{ $$.parse = prules( P0, P0 ); }
	| rules rule
		{ $$.parse = prules( $1.parse, $2.parse ); }
	;

rule	: INCLUDE args _SEMIC
		{ $$.parse = pincl( $2.list ); }
	| ARG args _SEMIC
		{ $$.parse = prule( $1.string, $2.list, L0 ); }
	| ARG args _COLON args _SEMIC
		{ $$.parse = prule( $1.string, $2.list, $4.list ); }
	| arg1 assign args _SEMIC
		{ $$.parse = pset( $1.list, $3.list, $2.number ); }
	| arg1 ON args assign args _SEMIC
		{ $$.parse = pstng( $3.list, $1.list, $5.list, $4.number ); }
	| arg1 DEFAULT _EQUALS args _SEMIC
		{ $$.parse = pset( $1.list, $4.list, ASSIGN_DEFAULT ); }
	| FOR ARG IN args _LBRACE rules _RBRACE
		{ $$.parse = pfor( $2.string, $6.parse, $4.list ); }
	| SWITCH args _LBRACE cases _RBRACE
		{ $$.parse = pswitch( $2.list, $4.parse ); }
	| IF cond _LBRACE rules _RBRACE 
		{ $$.parse = pif( $2.parse, pthen( $4.parse, P0 ) ); }
	| IF cond _LBRACE rules _RBRACE ELSE rule
		{ $$.parse = pif( $2.parse, pthen( $4.parse, $7.parse ) ); }
	| RULE ARG rule
		{ $$.parse = psetc( $2.string, $3.parse ); }
	| ACTIONS eflags ARG 
		{ yymode( SCAN_STRING ); }
	  STRING 
		{ $$.parse = psete( $3.string, $5.string, $2.number );
		  yymode( SCAN_NORMAL ); }
	| _LBRACE rules _RBRACE
		{ $$.parse = $2.parse; }
	;

/*
 * assign - = or +=
 */

assign	: _EQUALS
		{ $$.number = ASSIGN_SET; }
	| _PLUS_EQUALS
		{ $$.number = ASSIGN_APPEND; }
	| _QUESTION_EQUALS
		{ $$.number = ASSIGN_DEFAULT; }
	;

/*
 * cond - a conditional for 'if'
 */

cond	: arg1 
		{ $$.parse = pcomp( COND_EXISTS, $1.list, L0 ); }
	| arg1 _EQUALS arg1 
		{ $$.parse = pcomp( COND_EQUALS, $1.list, $3.list ); }
	| arg1 _BANG_EQUALS arg1
		{ $$.parse = pcomp( COND_NOTEQ, $1.list, $3.list ); }
	| arg1 _LANGLE arg1
		{ $$.parse = pcomp( COND_LESS, $1.list, $3.list ); }
	| arg1 _LANGLE_EQUALS arg1 
		{ $$.parse = pcomp( COND_LESSEQ, $1.list, $3.list ); }
	| arg1 _RANGLE arg1 
		{ $$.parse = pcomp( COND_MORE, $1.list, $3.list ); }
	| arg1 _RANGLE_EQUALS arg1 
		{ $$.parse = pcomp( COND_MOREEQ, $1.list, $3.list ); }
	| _BANG cond
		{ $$.parse = pcond( COND_NOT, $2.parse, P0 ); }
	| cond _AMPERAMPER cond 
		{ $$.parse = pcond( COND_AND, $1.parse, $3.parse ); }
	| cond _BARBAR cond
		{ $$.parse = pcond( COND_OR, $1.parse, $3.parse ); }
	| _LPAREN cond _RPAREN
		{ $$.parse = $2.parse; }
	;

/*
 * cases - action elements inside a 'switch'
 * case - a single action element inside a 'switch'
 *
 * Unfortunately, a right-recursive rule.
 */

cases	: /* empty */
		{ $$.parse = P0; }
	| case cases
		{ $$.parse = pcases( $1.parse, $2.parse ); }
	;

case	: CASE ARG _COLON rules
		{ $$.parse = pcase( $2.string, $4.parse ); }
	;

/*
 * args - zero or more ARGs in a LIST
 * arg1 - exactly one ARG in a LIST 
 */

args	: argsany
		{ yymode( SCAN_NORMAL ); }
	;

argsany	: /* empty */
		{ $$.list = L0; yymode( SCAN_PUNCT ); }
	| argsany ARG
		{ $$.list = list_new( $1.list, copystr( $2.string ) ); }
	;

arg1	: ARG 
		{ $$.list = list_new( L0, copystr( $1.string ) ); }
	;

/*
 * eflags - zero or more modifiers to 'executes'
 * eflag - a single modifier to 'executes'
 */

eflags	: /* empty */
		{ $$.number = 0; }
	| eflags eflag
		{ $$.number = $1.number | $2.number; }
	;

eflag	: UPDATED
		{ $$.number = EXEC_UPDATED; }
	| TOGETHER
		{ $$.number = EXEC_TOGETHER; }
	| IGNORE
		{ $$.number = EXEC_IGNORE; }
	| QUIETLY
		{ $$.number = EXEC_QUIETLY; }
	| PIECEMEAL
		{ $$.number = EXEC_PIECEMEAL; }
	| EXISTING
		{ $$.number = EXEC_EXISTING; }
	;

