/*
 * A simple mini-Chapel parser developed from Shannon's
 * experiments teaching herself flex and bison, and used
 * to exercise a prototype Chapel AST.
 *
 * Brad Chamberlain, 6/2004
 *
 */

%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexyacc.h"

%}

%union	{
  long intval;
  binOpType bot;
  Expr* pexpr;
  DomainExpr* pdexpr;
  Stmt* stmt;
  Type* pdt;
  char* pch;
  Symbol* psym;
}


%token CONFIG STATIC
%token VAR CONST

%token BOOLEAN INTEGER FLOAT
%token LOCALE TIMER

%token DOMAIN
%token INDEX

%token ENUM TYPE
%token FUNCTION

%token IDENT
%token DEFINED_IDENT
%token INTLITERAL FLOATLITERAL STRINGLITERAL

%token IF ELSE ELSIF
%token FOR FORALL IN
%token WHILE REPEAT UNTIL DO
%token RETURN

%token BY
%token ELLIPSIS

%token GETS PLUSGETS MINUSGETS TIMESGETS DIVGETS LSHGETS RSHGETS

%token SUM DIM REDUCE

%token EQUALS NEQUALS LEQUALS GEQUALS NEQUALS GTHAN LTHAN
%token LOGOR LOGAND
%token BITOR BITAND BITXOR
%token EXP


%type <intval> intliteral
%type <bot> binop otherbinop
%type <pdt> type types domainType indexType arrayType tupleType weirdType
%type <pdt> vardecltype
%type <pch> identifier 
%type <psym> idlist
%type <pexpr> expr exprlist nonemptyExprlist arrayref literal range
%type <pexpr> reduction memberaccess vardeclinit cast reduceDim
%type <pdexpr> domainExpr
%type <stmt> program statements statement decl vardecl assignment conditional 
%type <stmt> return loop forloop whileloop


/* These are declared in increasing order of precedence. */

%left EQUALS
%left '<' '>' 
%left '+' '-'
%left '*' '/'
%right '^'
%right UMINUS

%% 


program:  
  statements
    { program = $$; }
;


vardecltag:
  /* nothing */
| CONFIG
| STATIC
;


varconst:  
  VAR
| CONST
;


idlist:
  identifier
    { $$ = new Symbol($1); }
| idlist ',' identifier
    {
      $1->append(new Symbol($3));
      $$ = $1;
    }
;


vardecltype:
  /* nothing */
    { $$ = NULL; }
| ':' type
    { $$ = $2; }
;


vardeclinit:
  /* nothing */
    { $$ = new NullExpr(); }
| GETS expr
    { $$ = $2; }
;


vardecl:
  vardecltag varconst idlist vardecltype vardeclinit ';'
    {
      $3->setType($4);
      $$ = new VarDefStmt($3, $5);
    }
;


typedecl:
  typealias
| enumdecl
;

typealias:
  TYPE identifier GETS type ';'
    { Symboltable::define(new Symbol($2)); }
;

enumdecl:
  ENUM identifier GETS enumList ';'
    { Symboltable::define(new Symbol($2)); }
;

enumList:
  identifier
| enumList BITOR identifier
;


formal:
  identifier
;


nonemptyformals:
  formal
| nonemptyformals ',' formal
;

formals:
  /* empty */
| nonemptyformals
;


fndecl:
  FUNCTION identifier '(' formals ')' statement
| FUNCTION identifier '(' formals ')' ':' type statement
;


decl:
  vardecl
| typedecl
    { $$ = new NullStmt(); }
| fndecl
    { $$ = new NullStmt(); }
;


types:
  type
| types ',' type
    { 
      $1->append($3);
      $$ = $1;
    }
;


tupleType:
  '(' types ')'
    { $$ = $2; }
;


type:
  BOOLEAN
    { $$ = dtBoolean; }
| INTEGER
    { $$ = dtInteger; }
| FLOAT
    { $$ = dtFloat; }
| domainType
| indexType
| arrayType
| tupleType
| weirdType
| DEFINED_IDENT
    { $$ = NULL; }
;


weirdType:
  LOCALE
    { $$ = dtLocale; }
| TIMER
    { $$ = dtTimer; }
;


domainType:
  DOMAIN
    { $$ = new DomainType(); }
| DOMAIN '(' intliteral ')'
    { $$ = new DomainType($3); }
| DOMAIN '(' identifier ')'
    { $$ = new DomainType(777); }
;


indexType:
  INDEX
    { $$ = new DomainType(); }
| INDEX '(' intliteral ')'
    { $$ = new DomainType($3); }
| INDEX '(' identifier ')'
    { $$ = new DomainType(777); }
;


arrayType:
  '[' ']' type
    { $$ = new ArrayType(NULL, $3); }
| '[' domainExpr ']' type
    { $$ = new ArrayType($2, $4); }
;


statements:
  /* empty */
    { $$ = new NullStmt(); }
| statements statement
    {
      if ($1->isNull()) {
        $$ = $2;
      } else {
        $1->append($2);
        $$ = $1;
      }
    }
;


statement:
  decl
| assignment
| conditional
| loop
| expr ';'
    { $$ = new ExprStmt($1); }
| return
| '{' statements '}'
    { $$ = new LoopStmt($2); }
;


return:
  RETURN ';'
    { $$ = new ReturnStmt(NULL); }
| RETURN expr ';'
    { $$ = new ReturnStmt($2); }
;


fortype:
  FOR
| FORALL
;


forloop:
  fortype expr IN expr statement
    { $$ = new ForLoopStmt(true, $2, $4, $5); }
;


whileloop:
  WHILE expr statement
    { $$ = new NullStmt(); }
| DO statement WHILE expr ';'
    { $$ = new NullStmt(); }
| REPEAT statement UNTIL expr ';'
    { $$ = new NullStmt(); }
;


loop:
  forloop
| whileloop
;


conditional:
  IF expr statement
    { printf("conditional 1\n"); }
| IF expr statement ELSE statement
    { printf("conditional 2\n"); }
| IF expr statement elsifstmt
    { printf("conditional 3\n"); }
;

elsifstmt:
  ELSIF expr statement
    { printf("conditional 4\n"); }
| ELSIF expr statement ELSE statement
    { printf("conditional 5\n"); }
| ELSIF expr statement elsifstmt
    { printf("conditional 6\n"); }
;


/*
conditional:
  IF expr statement
    {printf("found an if-then statement\n");}
| IF expr withElse ELSE statement
    {printf("found an if-then-else statement\n");}
;


withElse:
  IF expr withElse ELSE withElse
| statement
;
*/


assignOp:
  GETS
| PLUSGETS
| MINUSGETS
| TIMESGETS
| DIVGETS
| LSHGETS
| RSHGETS
;


assignment:
  expr assignOp expr ';'
    { $$ = new ExprStmt(new AssignOp($1, $3)); }
;


exprlist:
  /* empty */
    { $$ = new NullExpr(); }
| nonemptyExprlist
;


nonemptyExprlist:
  expr
| exprlist ',' expr
    { $1->append($3); }
;


expr: 
  literal
| identifier
    { $$ = new Variable(new Symbol($1)); }
| expr binop expr
    { $$ = new BinOp($2, $1, $3); }
| expr otherbinop expr
| unop expr
    { $$ = new UnOp($2); }
| reduction
| arrayref
| cast
| memberaccess
| range
| '(' nonemptyExprlist ')' 
    { $$ = $2; }
| '[' domainExpr ']'
    { $$ = $2; }
| '[' domainExpr ']' expr
    { 
      $2->setForallExpr($4);
      $$ = $2;
    }
;


memberaccess:
  expr '.' expr
    { $$ = $1; }
;


reduceDim:
  /* empty */
    { $$ = new NullExpr(); }
| '(' expr ')'
    { $$ = $2; }
| '(' DIM GETS expr ')'
    { $$ = $4; }
;


reduction:
  SUM reduceDim expr
    { $$ = new ReduceExpr(pstSumReduce, $2, $3); }
| REDUCE reduceDim BY identifier expr
    { $$ = new ReduceExpr(new Symbol($4), $2, $5); }
;


domainExpr:
  nonemptyExprlist
    { $$ = new DomainExpr($1); }
| nonemptyExprlist ':' nonemptyExprlist
    { $$ = new DomainExpr($3, $1); }
;


range:
  expr ELLIPSIS expr
    { $$ = new SimpleSeqExpr($1, $3); }
| expr ELLIPSIS expr BY expr
    { $$ = new SimpleSeqExpr($1, $3, $5); }
| '*'
    { $$ = new FloodExpr(); }
| ELLIPSIS
    { $$ = new CompleteDimExpr(); }
;


literal:
  intliteral
    { $$ = new IntLiteral($1); }
| FLOATLITERAL
    { $$ = new FloatLiteral(atof(yytext)); }
| STRINGLITERAL
    { $$ = new StringLiteral("gooper"); }
;


intliteral:
  INTLITERAL
    { $$ = atol(yytext); }
;


unop:
  '+'
| '-'
| '!'
| '~'
;


binop:
  '+'
    { $$ = BINOP_PLUS; }
| '-'
    { $$ = BINOP_MINUS; }
| '*'
    { $$ = BINOP_MULT; }
| '/'
    { $$ = BINOP_DIV; }
| '%'
    { $$ = BINOP_MOD; }
| EQUALS
    { $$ = BINOP_EQUAL; }
| GETS
    { $$ = BINOP_EQUAL; }
| LEQUALS
    { $$ = BINOP_LEQUAL; }
| GEQUALS
    { $$ = BINOP_GEQUAL; }
| GTHAN
    { $$ = BINOP_GTHAN; }
| LTHAN
    { $$ = BINOP_LTHAN; }
| NEQUALS
    { $$ = BINOP_NEQUALS; }
| BITAND
    { $$ = BINOP_BITAND; }
| BITOR
    { $$ = BINOP_BITOR; }
| BITXOR
    { $$ = BINOP_BITXOR; }
| LOGAND
    { $$ = BINOP_LOGAND; }
| LOGOR
    { $$ = BINOP_LOGOR; }
| EXP
    { $$ = BINOP_EXP; }
;


otherbinop:
  BY
    { $$ = BINOP_OTHER; }
;


identifier:
  IDENT
    { $$ = copystring(yytext); }
;


arrayref:
  expr '(' exprlist ')'
    { $$ = new ParenOpExpr($1, $3); }
;


cast:
  type '(' exprlist ')'
    { $$ = new CastExpr($1, $3); }
;

%%
