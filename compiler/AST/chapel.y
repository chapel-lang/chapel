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


%token CONFIG STATIC
%token VAR CONST

%token BOOLEAN INTEGER FLOAT
%token LOCALE TIMER

%token DOMAIN

%token ENUM TYPE
%token FUNCTION

%token IDENT
%token DEFINED_IDENT
%token INTLITERAL FLOATLITERAL STRINGLITERAL

%token IF ELSE ELSIF
%token FOR FORALL IN
%token RETURN

%token BY
%token ELLIPSIS

%token EQUALS

%token SUM DIM

%token EQUALS NEQUALS LEQUALS GEQUALS NEQUALS GTHAN LTHAN
%token LOGOR LOGAND
%token BITOR BITAND BITXOR


%type <intval> intliteral
%type <bot> binop otherbinop
%type <pdt> type domainType arrayType weirdType
%type <pch> identifier
%type <pexpr> expr exprlist nonemptyExprlist arrayref literal range
%type <pexpr> reduction memberaccess
%type <pdexpr> domainExpr
%type <stmt> program statements statement decl vardecl assignment conditional 
%type <stmt> return loop


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


vardecltype:
  /* nothing */
| CONFIG
| STATIC
;


varconst:  
  VAR
| CONST
;


vardecl:
  vardecltype varconst identifier ':' type ';'
    { $$ = new VarDefStmt(new Symbol($3, $5), NULL); }
| vardecltype varconst identifier ':' type '=' expr ';'
    { $$ = new VarDefStmt(new Symbol($3, $5), $7); }
;


typedecl:
  typealias
| enumdecl
;

typealias:
  TYPE identifier '=' type ';'
    { Symboltable::define(new Symbol($2)); }
;

enumdecl:
  ENUM identifier '=' enumList ';'
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


type:
  BOOLEAN
    { $$ = dtBoolean; }
| INTEGER
    { $$ = dtInteger; }
| FLOAT
    { $$ = dtFloat; }
|  domainType
|  arrayType
|  weirdType
|  DEFINED_IDENT
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


loop:
  fortype expr IN expr statement
    { $$ = new ForLoopStmt(true, $2, $4, $5); }
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
| expr otherbinop expr
| unop expr
    { $$ = new UnOp($2); }
| reduction
| arrayref
| memberaccess
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


memberaccess:
  expr '.' identifier '(' ')'
    { $$ = $1; }
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


%%
