#ifndef lint
static char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define yyclearin (yychar=(-1))
#define yyerrok (yyerrflag=0)
#define YYRECOVERING (yyerrflag!=0)
#define YYPREFIX "yy"
#line 62 "jamgram.y"
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

#line 42 "y.tab.c"
#define _BANG 257
#define _BANG_EQUALS 258
#define _AMPERAMPER 259
#define _LPAREN 260
#define _RPAREN 261
#define _PLUS_EQUALS 262
#define _COLON 263
#define _SEMIC 264
#define _LANGLE 265
#define _LANGLE_EQUALS 266
#define _EQUALS 267
#define _RANGLE 268
#define _RANGLE_EQUALS 269
#define _QUESTION_EQUALS 270
#define ACTIONS 271
#define CASE 272
#define DEFAULT 273
#define ELSE 274
#define EXISTING 275
#define FOR 276
#define IF 277
#define IGNORE 278
#define IN 279
#define INCLUDE 280
#define ON 281
#define PIECEMEAL 282
#define QUIETLY 283
#define RULE 284
#define SWITCH 285
#define TOGETHER 286
#define UPDATED 287
#define _LBRACE 288
#define _BARBAR 289
#define _RBRACE 290
#define ARG 291
#define STRING 292
#define YYERRCODE 256
short yylhs[] = {                                        -1,
    0,    0,    2,    2,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    9,    1,    1,    5,    5,
    5,    7,    7,    7,    7,    7,    7,    7,    7,    7,
    7,    7,    6,    6,   10,    3,   11,   11,    4,    8,
    8,   12,   12,   12,   12,   12,   12,
};
short yylen[] = {                                         2,
    0,    2,    0,    2,    3,    3,    5,    4,    6,    5,
    7,    5,    5,    7,    3,    0,    5,    3,    1,    1,
    1,    1,    3,    3,    3,    3,    3,    3,    2,    3,
    3,    3,    0,    2,    4,    1,    0,    2,    1,    0,
    2,    1,    1,    1,    1,    1,    1,
};
short yydefred[] = {                                      1,
    0,   40,    0,    0,   37,    0,   37,    3,    0,    2,
    0,    0,    0,    0,    0,   39,    0,    0,    0,    0,
    0,    0,    0,    0,   20,   19,   21,    0,   37,   37,
   47,   44,   46,   45,   43,   42,   16,   41,   37,   29,
    0,    0,    0,    0,    0,    0,    0,    0,    3,    0,
    5,   38,   15,    0,   18,    4,   37,    6,   37,    0,
    0,    0,    0,   32,   24,   25,   26,   23,   27,   28,
   30,    0,    0,    0,    0,    0,    0,    0,   37,    8,
   17,    3,    0,    0,   12,   34,    7,   10,    0,    0,
    0,    3,    9,   11,   14,    0,
};
short yydgoto[] = {                                       1,
   56,   23,   19,   11,   30,   75,   18,   12,   62,   76,
   20,   38,
};
short yysindex[] = {                                      0,
 -141,    0, -281, -249,    0, -278,    0,    0,    0,    0,
 -125, -129, -264, -249, -249,    0,  -99, -256, -243, -272,
 -141, -265, -191, -202,    0,    0,    0, -240,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 -233, -262, -262, -262, -262, -262, -262, -249,    0, -249,
    0,    0,    0, -241,    0,    0,    0,    0,    0, -194,
 -229, -254, -248,    0,    0,    0,    0,    0,    0,    0,
    0, -173, -195, -234, -223, -241, -185, -183,    0,    0,
    0,    0, -190, -181,    0,    0,    0,    0, -176, -157,
 -141,    0,    0,    0,    0, -141,
};
short yyrindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0, -204,    0,
    0,    0,    0,    0,    0,    0, -245,    0,    0, -192,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -207,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, -252,    0,    0, -207,    0,    0,    0,    0,
    0,    0,    1,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -260,
};
short yygindex[] = {                                      0,
   -1,  -27,   -5,    3,   31,   16,   -9,    0,    0,    0,
    0,    0,
};
#define YYTABLESIZE 292
short yytable[] = {                                      10,
   13,   22,   48,   24,   40,   41,   17,   14,   31,   13,
   15,   35,   21,   22,   39,   22,   17,   17,   52,   53,
   51,   72,   54,   60,   61,   48,   59,   64,   16,   35,
   74,   49,   50,   63,   80,   31,   31,   81,   71,   82,
   73,   16,   22,   22,   65,   66,   67,   68,   69,   70,
   17,   77,   17,   78,   90,   50,   84,   39,   37,   37,
   57,   58,   39,   48,   96,   39,   85,   25,   39,   36,
   36,   36,   26,   89,   36,   27,   39,   36,   87,    2,
   88,   92,   33,   91,    3,    4,   37,   93,    5,   95,
   79,   86,    6,    7,    0,   36,    8,    2,   55,    9,
    0,    0,    3,    4,    0,    0,    5,    0,    0,    0,
    6,    7,    0,    2,    8,    0,   83,    9,    3,    4,
    0,    0,    5,    0,    0,    0,    6,    7,    0,    2,
    8,    0,   94,    9,    3,    4,   25,    0,    5,    0,
    0,   26,    6,    7,   27,   31,    8,   28,   32,    9,
    0,    0,   33,   34,    0,   29,   35,   36,   42,    0,
    0,   37,    0,    0,    0,   43,   44,   45,   46,   47,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   13,   13,    0,    0,    0,   13,   13,    0,    0,
   13,    0,    0,    0,   13,   13,    0,    0,   13,    0,
   13,   13,
};
short yycheck[] = {                                       1,
    0,    7,  259,    9,   14,   15,    4,  257,  261,  291,
  260,  272,  291,  259,  279,  261,   14,   15,  291,   21,
  264,   49,  288,   29,   30,  259,  267,  261,  291,  290,
  272,  288,  289,   39,  264,  288,  289,  292,   48,  288,
   50,  291,  288,  289,   42,   43,   44,   45,   46,   47,
   48,   57,   50,   59,   82,  289,  291,  262,  263,  264,
  263,  264,  267,  259,   92,  270,  290,  262,  273,  262,
  263,  264,  267,   79,  267,  270,  281,  270,  264,  271,
  264,  263,  290,  274,  276,  277,  291,  264,  280,   91,
   60,   76,  284,  285,   -1,  288,  288,  271,  290,  291,
   -1,   -1,  276,  277,   -1,   -1,  280,   -1,   -1,   -1,
  284,  285,   -1,  271,  288,   -1,  290,  291,  276,  277,
   -1,   -1,  280,   -1,   -1,   -1,  284,  285,   -1,  271,
  288,   -1,  290,  291,  276,  277,  262,   -1,  280,   -1,
   -1,  267,  284,  285,  270,  275,  288,  273,  278,  291,
   -1,   -1,  282,  283,   -1,  281,  286,  287,  258,   -1,
   -1,  291,   -1,   -1,   -1,  265,  266,  267,  268,  269,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  271,  272,   -1,   -1,   -1,  276,  277,   -1,   -1,
  280,   -1,   -1,   -1,  284,  285,   -1,   -1,  288,   -1,
  290,  291,
};
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 292
#if YYDEBUG
char *yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"_BANG","_BANG_EQUALS",
"_AMPERAMPER","_LPAREN","_RPAREN","_PLUS_EQUALS","_COLON","_SEMIC","_LANGLE",
"_LANGLE_EQUALS","_EQUALS","_RANGLE","_RANGLE_EQUALS","_QUESTION_EQUALS",
"ACTIONS","CASE","DEFAULT","ELSE","EXISTING","FOR","IF","IGNORE","IN","INCLUDE",
"ON","PIECEMEAL","QUIETLY","RULE","SWITCH","TOGETHER","UPDATED","_LBRACE",
"_BARBAR","_RBRACE","ARG","STRING",
};
char *yyrule[] = {
"$accept : stmts",
"stmts :",
"stmts : stmts rule",
"rules :",
"rules : rules rule",
"rule : INCLUDE args _SEMIC",
"rule : ARG args _SEMIC",
"rule : ARG args _COLON args _SEMIC",
"rule : arg1 assign args _SEMIC",
"rule : arg1 ON args assign args _SEMIC",
"rule : arg1 DEFAULT _EQUALS args _SEMIC",
"rule : FOR ARG IN args _LBRACE rules _RBRACE",
"rule : SWITCH args _LBRACE cases _RBRACE",
"rule : IF cond _LBRACE rules _RBRACE",
"rule : IF cond _LBRACE rules _RBRACE ELSE rule",
"rule : RULE ARG rule",
"$$1 :",
"rule : ACTIONS eflags ARG $$1 STRING",
"rule : _LBRACE rules _RBRACE",
"assign : _EQUALS",
"assign : _PLUS_EQUALS",
"assign : _QUESTION_EQUALS",
"cond : arg1",
"cond : arg1 _EQUALS arg1",
"cond : arg1 _BANG_EQUALS arg1",
"cond : arg1 _LANGLE arg1",
"cond : arg1 _LANGLE_EQUALS arg1",
"cond : arg1 _RANGLE arg1",
"cond : arg1 _RANGLE_EQUALS arg1",
"cond : _BANG cond",
"cond : cond _AMPERAMPER cond",
"cond : cond _BARBAR cond",
"cond : _LPAREN cond _RPAREN",
"cases :",
"cases : case cases",
"case : CASE ARG _COLON rules",
"args : argsany",
"argsany :",
"argsany : argsany ARG",
"arg1 : ARG",
"eflags :",
"eflags : eflags eflag",
"eflag : UPDATED",
"eflag : TOGETHER",
"eflag : IGNORE",
"eflag : QUIETLY",
"eflag : PIECEMEAL",
"eflag : EXISTING",
};
#endif
#ifndef YYSTYPE
typedef int YYSTYPE;
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 500
#define YYMAXDEPTH 500
#endif
#endif
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short yyss[YYSTACKSIZE];
YYSTYPE yyvs[YYSTACKSIZE];
#define yystacksize YYSTACKSIZE
#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
yyparse()
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register char *yys;
    extern char *getenv();

    if (yys = getenv("YYDEBUG"))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if (yyn = yydefred[yystate]) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yyss + yystacksize - 1)
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#ifdef lint
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#ifdef lint
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yyss + yystacksize - 1)
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 1:
#line 100 "jamgram.y"
{
			compile_builtins();
		}
break;
case 2:
#line 104 "jamgram.y"
{ 
			(*(yyvsp[0].parse->func))( yyvsp[0].parse, L0, L0 );
			parse_free( yyvsp[0].parse );
		}
break;
case 3:
#line 116 "jamgram.y"
{ yyval.parse = prules( P0, P0 ); }
break;
case 4:
#line 118 "jamgram.y"
{ yyval.parse = prules( yyvsp[-1].parse, yyvsp[0].parse ); }
break;
case 5:
#line 122 "jamgram.y"
{ yyval.parse = pincl( yyvsp[-1].list ); }
break;
case 6:
#line 124 "jamgram.y"
{ yyval.parse = prule( yyvsp[-2].string, yyvsp[-1].list, L0 ); }
break;
case 7:
#line 126 "jamgram.y"
{ yyval.parse = prule( yyvsp[-4].string, yyvsp[-3].list, yyvsp[-1].list ); }
break;
case 8:
#line 128 "jamgram.y"
{ yyval.parse = pset( yyvsp[-3].list, yyvsp[-1].list, yyvsp[-2].number ); }
break;
case 9:
#line 130 "jamgram.y"
{ yyval.parse = pstng( yyvsp[-3].list, yyvsp[-5].list, yyvsp[-1].list, yyvsp[-2].number ); }
break;
case 10:
#line 132 "jamgram.y"
{ yyval.parse = pset( yyvsp[-4].list, yyvsp[-1].list, ASSIGN_DEFAULT ); }
break;
case 11:
#line 134 "jamgram.y"
{ yyval.parse = pfor( yyvsp[-5].string, yyvsp[-1].parse, yyvsp[-3].list ); }
break;
case 12:
#line 136 "jamgram.y"
{ yyval.parse = pswitch( yyvsp[-3].list, yyvsp[-1].parse ); }
break;
case 13:
#line 138 "jamgram.y"
{ yyval.parse = pif( yyvsp[-3].parse, pthen( yyvsp[-1].parse, P0 ) ); }
break;
case 14:
#line 140 "jamgram.y"
{ yyval.parse = pif( yyvsp[-5].parse, pthen( yyvsp[-3].parse, yyvsp[0].parse ) ); }
break;
case 15:
#line 142 "jamgram.y"
{ yyval.parse = psetc( yyvsp[-1].string, yyvsp[0].parse ); }
break;
case 16:
#line 144 "jamgram.y"
{ yymode( SCAN_STRING ); }
break;
case 17:
#line 146 "jamgram.y"
{ yyval.parse = psete( yyvsp[-2].string, yyvsp[0].string, yyvsp[-3].number );
		  yymode( SCAN_NORMAL ); }
break;
case 18:
#line 149 "jamgram.y"
{ yyval.parse = yyvsp[-1].parse; }
break;
case 19:
#line 157 "jamgram.y"
{ yyval.number = ASSIGN_SET; }
break;
case 20:
#line 159 "jamgram.y"
{ yyval.number = ASSIGN_APPEND; }
break;
case 21:
#line 161 "jamgram.y"
{ yyval.number = ASSIGN_DEFAULT; }
break;
case 22:
#line 169 "jamgram.y"
{ yyval.parse = pcomp( COND_EXISTS, yyvsp[0].list, L0 ); }
break;
case 23:
#line 171 "jamgram.y"
{ yyval.parse = pcomp( COND_EQUALS, yyvsp[-2].list, yyvsp[0].list ); }
break;
case 24:
#line 173 "jamgram.y"
{ yyval.parse = pcomp( COND_NOTEQ, yyvsp[-2].list, yyvsp[0].list ); }
break;
case 25:
#line 175 "jamgram.y"
{ yyval.parse = pcomp( COND_LESS, yyvsp[-2].list, yyvsp[0].list ); }
break;
case 26:
#line 177 "jamgram.y"
{ yyval.parse = pcomp( COND_LESSEQ, yyvsp[-2].list, yyvsp[0].list ); }
break;
case 27:
#line 179 "jamgram.y"
{ yyval.parse = pcomp( COND_MORE, yyvsp[-2].list, yyvsp[0].list ); }
break;
case 28:
#line 181 "jamgram.y"
{ yyval.parse = pcomp( COND_MOREEQ, yyvsp[-2].list, yyvsp[0].list ); }
break;
case 29:
#line 183 "jamgram.y"
{ yyval.parse = pcond( COND_NOT, yyvsp[0].parse, P0 ); }
break;
case 30:
#line 185 "jamgram.y"
{ yyval.parse = pcond( COND_AND, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 31:
#line 187 "jamgram.y"
{ yyval.parse = pcond( COND_OR, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 32:
#line 189 "jamgram.y"
{ yyval.parse = yyvsp[-1].parse; }
break;
case 33:
#line 200 "jamgram.y"
{ yyval.parse = P0; }
break;
case 34:
#line 202 "jamgram.y"
{ yyval.parse = pcases( yyvsp[-1].parse, yyvsp[0].parse ); }
break;
case 35:
#line 206 "jamgram.y"
{ yyval.parse = pcase( yyvsp[-2].string, yyvsp[0].parse ); }
break;
case 36:
#line 215 "jamgram.y"
{ yymode( SCAN_NORMAL ); }
break;
case 37:
#line 219 "jamgram.y"
{ yyval.list = L0; yymode( SCAN_PUNCT ); }
break;
case 38:
#line 221 "jamgram.y"
{ yyval.list = list_new( yyvsp[-1].list, copystr( yyvsp[0].string ) ); }
break;
case 39:
#line 225 "jamgram.y"
{ yyval.list = list_new( L0, copystr( yyvsp[0].string ) ); }
break;
case 40:
#line 234 "jamgram.y"
{ yyval.number = 0; }
break;
case 41:
#line 236 "jamgram.y"
{ yyval.number = yyvsp[-1].number | yyvsp[0].number; }
break;
case 42:
#line 240 "jamgram.y"
{ yyval.number = EXEC_UPDATED; }
break;
case 43:
#line 242 "jamgram.y"
{ yyval.number = EXEC_TOGETHER; }
break;
case 44:
#line 244 "jamgram.y"
{ yyval.number = EXEC_IGNORE; }
break;
case 45:
#line 246 "jamgram.y"
{ yyval.number = EXEC_QUIETLY; }
break;
case 46:
#line 248 "jamgram.y"
{ yyval.number = EXEC_PIECEMEAL; }
break;
case 47:
#line 250 "jamgram.y"
{ yyval.number = EXEC_EXISTING; }
break;
#line 633 "y.tab.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yyss + yystacksize - 1)
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
