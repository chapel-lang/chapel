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
}


%token VAR CONST

%token INTEGER FLOAT
%token LOCALE

%token DOMAIN

%token IDENT
%token INTLITERAL FLOATLITERAL

%token IF THEN ELSE
%token FORALL IN
%token RETURN

%token BY
%token ELLIPSIS

%token EQUALS

%token SUM DIM

%token EQUALS NEQUALS LEQUALS GEQUALS NEQUALS GTHAN LTHAN
%token LOGOR LOGAND
%token BITOR BITAND BITXOR


%type <intval> intliteral
%type <bot> binop
%type <pdt> type domainType arrayType
%type <pch> identifier
%type <pexpr> expr exprlist nonemptyExprlist arrayref literal range
%type <pexpr> reduction
%type <pdexpr> domainExpr
%type <stmt> program statements statement decl assignment conditional loop


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


varconst:  
  VAR
| CONST
;


decl:
  varconst identifier ':' type ';'
    { $$ = new VarDefStmt(new Symbol($2, $4), NULL); }
| varconst identifier ':' type '=' expr ';'
    { $$ = new VarDefStmt(new Symbol($2, $4), $6); }
;


type:  
  INTEGER
    { $$ = dtInteger; }
| FLOAT
    { $$ = dtFloat; }
|  domainType
|  arrayType
;


domainType:
  DOMAIN
    { $$ = new DomainType(); }
| DOMAIN '(' intliteral ')'
    { $$ = new DomainType($3); }
;


arrayType:
  '[' ']' type
    { $$ = new ArrayType(NULL, $3); }
| '[' expr ']' type
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
| expr
    { $$ = new ExprStmt($1); }
| '{' statements '}'
    { $$ = new LoopStmt($2); }
;


loop:
  FORALL expr IN expr statement
    { $$ = new ForLoopStmt(true, $2, $4, $5); }
;


conditional:
  IF expr THEN statement
    {printf("found an if-then statement\n");}
| IF expr THEN withElse ELSE statement
    {printf("found an if-then-else statement\n");}
;


withElse:
  IF expr THEN withElse ELSE withElse
| assignment
;


assignment:
  expr '=' expr ';'
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
| unop expr
    { $$ = new UnOp($2); }
| reduction
| arrayref
| range
| '(' expr ')' 
    { $$ = $2; }
| '[' domainExpr ']'
    { $$ = $2; }
| '[' domainExpr ']' expr
    { 
      $2->setForallExpr($4);
      $$ = $2;
    }
;


reduction:
  SUM '(' DIM '=' intliteral ')' expr
    { $$ = $7; }
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
;


identifier:
  IDENT
    { $$ = copystring(yytext); }
;


arrayref:
  expr '(' exprlist ')'
    { $$ = new ParenOpExpr($1, $3); }
;


%%
