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
  bool boolval;
  long intval;
  char* pch;

  unOpType uot;
  binOpType bot;
  varType vt;
  paramType pt;

  Expr* pexpr;
  DomainExpr* pdexpr;
  Stmt* stmt;
  Type* pdt;
  Symbol* psym;
  VarSymbol* pvsym;
  TypeSymbol* ptsym;
}


%token CONFIG STATIC
%token VAR CONST

%token BOOLEAN INTEGER FLOAT
%token LOCALE TIMER

%token DOMAIN
%token INDEX

%token ENUM TYPE
%token FUNCTION
%token INOUT IN OUT REF VAL

%token IDENT
%token <ptsym> DEFINED_IDENT
%token INTLITERAL FLOATLITERAL 
%token <pch> STRINGLITERAL

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


%type <boolval> varconst
%type <intval> intliteral

%type <bot> binop otherbinop
%type <uot> unop
%type <vt> vardecltag
%type <pt> formaltag

%type <pdt> type types domainType indexType arrayType tupleType weirdType
%type <pdt> vardecltype fnrettype
%type <pch> identifier 
%type <pvsym> idlist
%type <psym> enumList formal nonemptyformals formals
%type <pexpr> expr exprlist nonemptyExprlist arrayref literal range
%type <pexpr> reduction memberaccess vardeclinit cast reduceDim
%type <pdexpr> domainExpr
%type <stmt> program statements statement decl vardecl assignment conditional
%type <stmt> return loop forloop whileloop enumdecl typealias typedecl fndecl


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
    { $$ = VAR_NORMAL; }
| CONFIG
    { $$ = VAR_CONFIG; }
| STATIC
    { $$ = VAR_STATE; }
;


varconst:
  VAR
    { $$ = false; }
| CONST
    { $$ = true; }
;


idlist:
  identifier
    { $$ = new VarSymbol($1); }
| idlist ',' identifier
    {
      $1->append(new VarSymbol($3));
      $$ = $1;
    }
;


vardecltype:
  /* nothing */
    { $$ = dtUnknown; }
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
      $3->setIsConst($2);
      $3->setType($4);
      $3->setVarClass($1);
      $$ = new VarDefStmt($3, $5);
    }
;


typedecl:
  typealias
| enumdecl
;


typealias:
  TYPE identifier GETS type ';'
    {
      UserType* pdt = new UserType($4);
      Symbol* pst = new TypeSymbol($2, pdt);
      pdt->addName(pst);
      Symboltable::define(pst);
      $$ = new TypeDefStmt(pdt);
    }
;


enumdecl:
  ENUM identifier GETS enumList ';'
    {
      EnumType* pdt = new EnumType($4);
      Symbol* pst = new TypeSymbol($2, pdt);
      pdt->addName(pst);
      Symboltable::define(pst);
      $$ = new TypeDefStmt(pdt);
    }
;

enumList:
  identifier
    { $$ = new Symbol($1); }
| enumList BITOR identifier
    {
      $1->append(new Symbol($3));
      $$ = $1;
    }
;


formaltag:
  /* nothing */
    { $$ = PARAM_INOUT; }
| IN
    { $$ = PARAM_IN; }
| INOUT
    { $$ = PARAM_INOUT; }
| OUT
    { $$ = PARAM_OUT; }
| CONST
    { $$ = PARAM_CONST; }
| REF
    { $$ = PARAM_INOUT; }
| VAL
    { $$ = PARAM_IN; }
;


formal:
  formaltag idlist vardecltype
    { $$ = new ParamSymbol(PARAM_INOUT, "???", $3); }
;


nonemptyformals:
  formal
| nonemptyformals ';' formal
    {
      $1->append($3);
      $$ = $1;
    }
;

formals:
  /* empty */
    { $$ = new NullSymbol(); }
| nonemptyformals
;


fnrettype:
  /* empty */
    { $$ = new NullType(); }
| ':'
    { $$ = dtUnknown; }
| ':' type
    { $$ = $2; }
;


fndecl:
  FUNCTION identifier '(' formals ')' fnrettype statement
    {
      FunSymbol* fnpst = new FunSymbol($2, $4, $6, $7);
      $$ = new FnDefStmt(fnpst);
    }
;


decl:
  vardecl
| typedecl
| fndecl
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
    { $$ = $1->definition; }
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
    { $$ = new SubDomainType(new Symbol($3)); }
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
    { $$ = new ArrayType(unknownDomain, $3); }
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
    { $$ = new WhileLoopStmt(LOOP_WHILEDO, $2, $3); }
| DO statement WHILE expr ';'
    { $$ = new WhileLoopStmt(LOOP_DOWHILE, $4, $2); }
| REPEAT statement UNTIL expr ';'
    { $$ = new WhileLoopStmt(LOOP_REPEAT, $4, $2); }
;


loop:
  forloop
| whileloop
;


conditional:
  IF expr statement
    { $$ = new CondStmt($2, $3); }
| IF expr statement ELSE statement
    { $$ = new CondStmt($2, $3, $5); }
| IF expr statement elsifstmt
    { $$ = new CondStmt($2, $3); }  /* BLC: Todo: elsifstmt */
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
    { $$ = new Variable(new VarSymbol($1)); }
| expr binop expr
    { $$ = new BinOp($2, $1, $3); }
| expr otherbinop expr
    { $$ = new SpecialBinOp($2, $1, $3); }
| unop expr
    { $$ = new UnOp($1, $2); }
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
    { $$ = new SpecialBinOp(BINOP_DOT, $1, $3); }
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
    { $$ = new IntLiteral(yytext, $1); }
| FLOATLITERAL
    { $$ = new FloatLiteral(yytext, atof(yytext)); }
| STRINGLITERAL
    { $$ = new StringLiteral($1); }
;


intliteral:
  INTLITERAL
    { $$ = atol(yytext); }
;


unop:
  '+'
    { $$ = UNOP_PLUS; }
| '-'
    { $$ = UNOP_MINUS; }
| '!'
    { $$ = UNOP_LOGNOT; }
| '~'
    { $$ = UNOP_BITNOT; }
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
    { $$ = BINOP_BY; }
;


identifier:
  IDENT
    { $$ = copystring(yytext); }
| '?' IDENT
    { $$ = glomstrings(2, "?", yytext); }
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
