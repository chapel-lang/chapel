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

%start program

%union	{
  bool boolval;
  long intval;
  char* pch;

  unOpType uot;
  binOpType bot;
  getsOpType got;
  varType vt;
  paramType pt;

  Expr* pexpr;
  DomainExpr* pdexpr;
  Stmt* stmt;
  Type* pdt;
  Symbol* psym;
  VarSymbol* pvsym;
  TypeSymbol* ptsym;
  ReduceSymbol* redsym;
  ClassSymbol* pcsym;
}


%token CONFIG STATIC
%token VAR CONST

%token DOMAIN
%token INDEX

%token ENUM TYPEDEF CLASS
%token FUNCTION
%token INOUT IN OUT REF VAL

%token IDENT QUERY_IDENT REDUCE_IDENT
%token <ptsym> TYPE_IDENT
%token <redsym> REDUCE_IDENT
%token INTLITERAL FLOATLITERAL 
%token <pch> STRINGLITERAL

%token IF ELSE ELSIF
%token FOR FORALL IN
%token WHILE DO
%token RETURN

%token BY
%token ELLIPSIS

%token GETS PLUSGETS MINUSGETS TIMESGETS DIVGETS BITANDGETS BITORGETS 
%token BITXORGETS LSHGETS RSHGETS

%token DIM REDUCE

%token EQUALS NEQUALS LEQUALS GEQUALS NEQUALS GTHAN LTHAN
%token LOGOR LOGAND
%token BITOR BITAND BITXOR BITSL BITSR
%token EXP


%type <boolval> varconst
%type <intval> intliteral

%type <got> assignOp
%type <bot> otherbinop
%type <uot> unop
%type <vt> vardecltag
%type <pt> formaltag

%type <pdt> type types domainType indexType arrayType tupleType
%type <pdt> vardecltype fnrettype
%type <pch> identifier query_identifier
%type <psym> identsym enumList formal nonemptyformals formals idlist indexlist
%type <pcsym> subclass
%type <pexpr> expr exprlist nonemptyExprlist arrayfun literal range
%type <pexpr> reduction memberaccess vardeclinit cast reduceDim binop
%type <pdexpr> domainExpr
%type <stmt> program statements statement decl vardecl assignment conditional
%type <stmt> retStmt loop forloop whileloop enumdecl typealias typedecl fndecl
%type <stmt> classdecl


/* These are declared in increasing order of precedence. */

%left LOGOR
%left LOGAND
%right '!'
%left BITOR
%left BITXOR
%left BITAND
%left EQUALS NEQUALS
%left LTHAN LEQUALS GTHAN GEQUALS
%left '+' '-'
%right UMINUS '~'
%left '*' '/' '%'
%left EXP

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


identsym:
  identifier
    { $$ = new Symbol(SYMBOL, $1); }
;


idlist:
  identsym
| idlist ',' identsym
    {
      $1->append($3);
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
    { $$ = nilExpr; }
| GETS expr
    { $$ = $2; }
;


vardecl:
  vardecltag varconst idlist vardecltype vardeclinit ';'
    {
      VarSymbol* varList = Symboltable::defineVars($3, $4, $5, $1, $2);
      $$ = new VarDefStmt(varList, $5);
    }
;


typedecl:
  typealias
| enumdecl
| classdecl
;


typealias:
  TYPEDEF identifier GETS type ';'
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
      EnumSymbol* enumlist = Symboltable::defineEnumList($4);      
      EnumType* pdt = new EnumType(enumlist);
      Symbol* pst = new TypeSymbol($2, pdt);
      pdt->addName(pst);
      Symboltable::define(pst);
      $$ = new TypeDefStmt(pdt);
    }
;


subclass:
  /* nothing */
    { $$ = nilClassSymbol; }
| ':' identifier
    {
      $$ = Symboltable::lookupClass($2);
    }
;


classdecl:
  CLASS identifier subclass '{' '}'
    {
      ClassType* pdt = Symboltable::defineClass($2, $3);
      $$ = new TypeDefStmt(pdt);
    }
;


enumList:
  identsym
| enumList BITOR identsym
    {
      $1->append($3);
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
    { $$ = Symboltable::defineParams($1, $2, $3); }
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
    { $$ = nilSymbol; }
| nonemptyformals
;


fnrettype:
  /* empty */
    { $$ = dtVoid; }
| ':'
    { $$ = dtUnknown; }
| ':' type
    { $$ = $2; }
;


fndecl:
  FUNCTION identifier
    {
      Symboltable::pushScope(SCOPE_PARAM);
    }
                      '(' formals ')' fnrettype statement
    {
      Symboltable::popScope();
      FnSymbol* fnpst = Symboltable::defineFunction($2, $5, $7, $8);
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
  domainType
| indexType
| arrayType
| tupleType
| TYPE_IDENT
    { $$ = $1->type; }
| query_identifier
    { $$ = dtUnknown; }
;

domainType:
  DOMAIN
    { $$ = new DomainType(); }
| DOMAIN '(' intliteral ')'
    { $$ = new DomainType($3); }
| DOMAIN '(' identifier ')'
    { $$ = new SubDomainType(new Symbol(SYMBOL, $3)); }
;


indexType:
  INDEX
    { $$ = new IndexType(); }
| INDEX '(' intliteral ')'
    { $$ = new IndexType($3); }
| INDEX '(' identifier ')'
    { $$ = new SubIndexType(new Symbol(SYMBOL, $3)); }
;


arrayType:
  '[' ']' type
    { $$ = new ArrayType(unknownDomain, $3); }
| '[' query_identifier ']' type
    { 
      Symboltable::defineQueryDomain($2);  // really need to tuck this into
                                           // a var def stmt to be inserted
                                           // as soon as the next stmt is
                                           // defined  -- BLC
      $$ = new ArrayType(unknownDomain, $4);
    }
| '[' domainExpr ']' type
    { $$ = new ArrayType($2, $4); }
;


statements:
  /* empty */
    { $$ = nilStmt; }
| statements statement
    { $$ = appendLink($1, $2); }
;


statement:
  ';'
    { $$ = new NoOpStmt(); }
| decl
| assignment
| conditional
| loop
| expr ';'
    { $$ = new ExprStmt($1); }
| retStmt
| '{'
    { Symboltable::pushScope(SCOPE_LOCAL); }
      statements '}'
    {
      Symboltable::popScope();
      $$ = new BlockStmt($3);
    }
;


retStmt:
  RETURN ';'
    { $$ = new ReturnStmt(nilExpr); }
| RETURN expr ';'
    { $$ = new ReturnStmt($2); }
;


fortype:
  FOR
| FORALL
;


indexlist:
  idlist
| '(' idlist ')'
  { $$ = $2; }
;


forloop:
  fortype indexlist IN expr
    { 
      $<pvsym>$ = Symboltable::enterForLoop($2);
    }
                                 statement
    { 
      Symboltable::exitForLoop();
      $$ = new ForLoopStmt(true, $<pvsym>5, $4, $6);
    }
;


whileloop:
  WHILE expr statement
    { $$ = new WhileLoopStmt(true, $2, $3); }
| DO statement WHILE expr ';'
    { $$ = new WhileLoopStmt(false, $4, $2); }
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
    { $$ = GETS_NORM; }
| PLUSGETS
    { $$ = GETS_PLUS; }
| MINUSGETS
    { $$ = GETS_MINUS; }
| TIMESGETS
    { $$ = GETS_TIMES; }
| DIVGETS
    { $$ = GETS_DIV; }
| BITANDGETS
    { $$ = GETS_BITAND; }
| BITORGETS
    { $$ = GETS_BITOR; }
| BITXORGETS
    { $$ = GETS_BITXOR; }
| LSHGETS
    { $$ = GETS_LSH; }
| RSHGETS
    { $$ = GETS_RSH; }
;


assignment:
  expr assignOp expr ';'
    { $$ = new ExprStmt(new AssignOp($2, $1, $3)); }
;


exprlist:
  /* empty */
    { $$ = nilExpr; }
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
    { $$ = new Variable(Symboltable::lookup($1)); }
| binop
| expr otherbinop expr
    { $$ = new SpecialBinOp($2, $1, $3); }
| unop expr
    { $$ = new UnOp($1, $2); }
| reduction
| arrayfun
| cast
| memberaccess
| range
| '(' nonemptyExprlist ')' 
    { 
      if ($2->next->isNull()) {
        $$ = $2;
      } else {
        $$ = new Tuple($2);
      }
    }
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
    { $$ = nilExpr; }
| '(' expr ')'
    { $$ = $2; }
| '(' DIM GETS expr ')'
    { $$ = $4; }
;


reduction:
  REDUCE_IDENT reduceDim expr
    { $$ = new ReduceExpr($1, $2, $3); }
| REDUCE reduceDim BY REDUCE_IDENT expr
    { $$ = new ReduceExpr($4, $2, $5); }
;


domainExpr:
  nonemptyExprlist
    { $$ = new DomainExpr($1); }
| indexlist ':' nonemptyExprlist          // BLC: This is wrong vv
    { $$ = new DomainExpr($3, Symboltable::defineVars($1, dtInteger)); }
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
  expr '+' expr
    { $$ = new BinOp(BINOP_PLUS, $1, $3); }
| expr '-' expr
    { $$ = new BinOp(BINOP_MINUS, $1, $3); }
| expr '*' expr
    { $$ = new BinOp(BINOP_MULT, $1, $3); }
| expr '/' expr
    { $$ = new BinOp(BINOP_DIV, $1, $3); }
| expr '%' expr
    { $$ = new BinOp(BINOP_MOD, $1, $3); }
| expr EQUALS expr
    { $$ = new BinOp(BINOP_EQUAL, $1, $3); }
/*
| expr GETS expr
    { $$ = new BinOp(BINOP_EQUAL, $1, $3); }
*/
| expr LEQUALS expr
    { $$ = new BinOp(BINOP_LEQUAL, $1, $3); }
| expr GEQUALS expr
    { $$ = new BinOp(BINOP_GEQUAL, $1, $3); }
| expr GTHAN expr
    { $$ = new BinOp(BINOP_GTHAN, $1, $3); }
| expr LTHAN expr
    { $$ = new BinOp(BINOP_LTHAN, $1, $3); }
| expr NEQUALS expr
    { $$ = new BinOp(BINOP_NEQUALS, $1, $3); }
| expr BITAND expr
    { $$ = new BinOp(BINOP_BITAND, $1, $3); }
| expr BITOR expr
    { $$ = new BinOp(BINOP_BITOR, $1, $3); }
| expr BITXOR expr
    { $$ = new BinOp(BINOP_BITXOR, $1, $3); }
| expr BITSL expr
    { $$ = new BinOp(BINOP_BITSL, $1, $3); }
| expr BITSR expr
    { $$ = new BinOp(BINOP_BITSR, $1, $3); }
| expr LOGAND expr
    { $$ = new BinOp(BINOP_LOGAND, $1, $3); }
| expr LOGOR expr
    { $$ = new BinOp(BINOP_LOGOR, $1, $3); }
| expr EXP expr
    { $$ = new BinOp(BINOP_EXP, $1, $3); }
;


otherbinop:
  BY
    { $$ = BINOP_BY; }
;


identifier:
  IDENT
    { $$ = copystring(yytext); }
/*
| '?' IDENT
    { $$ = glomstrings(2, "?", yytext); }
*/
;


query_identifier:
  QUERY_IDENT
    { $$ = copystring(yytext+1); }
;


arrayfun:
  expr '(' exprlist ')'
    { $$ = ParenOpExpr::classify($1, $3); }
/*
| expr '[' exprlist ']'
    { $$ = ParenOpExpr::classify($1, $3); }
*/
;


cast:
  type '(' exprlist ')'
    { $$ = new CastExpr($1, $3); }
;

%%
