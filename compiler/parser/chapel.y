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

  getsOpType got;
  varType vt;
  paramType pt;

  Expr* pexpr;
  DomainExpr* pdexpr;
  Stmt* stmt;
  Type* pdt;
  TupleType* tupledt;
  Symbol* psym;
  VarSymbol* pvsym;
  TypeSymbol* ptsym;
  FnSymbol* fnsym;
}

%token TCALL
%token TCLASS
%token TCONFIG
%token TCONST
%token TDO
%token TDOMAIN
%token TENUM
%token TFOR
%token TFORALL
%token TFUNCTION
%token TIF
%token TIN
%token TINDEX
%token TINOUT
%token TOUT
%token TREF
%token TRETURN
%token TSTATIC
%token TTHEN
%token TTYPE
%token TVAL
%token TVAR
%token TWHILE

%token TIDENT QUERY_IDENT
%token <ptsym> TYPE_IDENT
%token INTLITERAL FLOATLITERAL COMPLEXLITERAL
%token <pch> STRINGLITERAL

%token TASSIGN;
%token TASSIGNPLUS;
%token TASSIGNMINUS;
%token TASSIGNMULTIPLY;
%token TASSIGNDIVIDE;
%token TASSIGNBAND;
%token TASSIGNBOR;
%token TASSIGNBXOR;

%token TSEMI;
%token TCOMMA;
%token TDOT;
%token TLP;
%token TRP;
%token TLSBR;
%token TRSBR;
%token TLCBR;
%token TRCBR;

%type <boolval> varconst
%type <intval> intliteral

%type <got> assignOp
%type <vt> vardecltag
%type <pt> formaltag

%type <pdt> type domainType indexType arrayType tupleType
%type <tupledt> tupleTypes
%type <pdt> vardecltype fnrettype
%type <pch> identifier query_identifier
%type <psym> identsym enumList formal nonemptyformals formals idlist indexlist subclass
%type <pexpr> simple_lvalue assign_lvalue lvalue atom expr exprlist nonemptyExprlist literal range
%type <pexpr> reduction vardeclinit
%type <pdexpr> domainExpr
%type <stmt> program statements statement decls decl vardecl assignment conditional
%type <stmt> retStmt loop forloop whileloop enumdecl typealias typedecl fndecl
%type <stmt> classdecl


/* These are declared in increasing order of precedence. */

%left TNOELSE
%left TELSE

%left TRSBR
%left TBY
%left TDOTDOT
%left TOR
%left TAND
%right TNOT
%left TEQUAL TNOTEQUAL
%left TLESSEQUAL TGREATEREQUAL TLESS TGREATER
%left TBOR
%left TBXOR
%left TBAND
%left TPLUS TMINUS
%left TSTAR TDIVIDE TMOD
%right TUPLUS TUMINUS TREDUCE TBNOT
%right TEXP
%left TCOLON

%% 


program:  
  statements
    { yystmtlist = $$; }
;


vardecltag:
  /* nothing */
    { $$ = VAR_NORMAL; }
| TCONFIG
    { $$ = VAR_CONFIG; }
| TSTATIC
    { $$ = VAR_STATE; }
;


varconst:
  TVAR
    { $$ = false; }
| TCONST
    { $$ = true; }
;


identsym:
  identifier
    { $$ = new Symbol(SYMBOL, $1); }
;


idlist:
  identsym
| idlist TCOMMA identsym
    {
      $1->append($3);
      $$ = $1;
    }
;


vardecltype:
  /* nothing */
    { $$ = dtUnknown; }
| TCOLON type
    { $$ = $2; }
;


vardeclinit:
  /* nothing */
    { $$ = nilExpr; }
| TASSIGN expr
    { $$ = $2; }
;


vardecl:
  vardecltag varconst idlist vardecltype vardeclinit TSEMI
    { $$ = Symboltable::defineVarDefStmt($3, $4, $5, $1, $2); }
;


typedecl:
  typealias
| enumdecl
| classdecl
;


typealias:
  TTYPE identifier TCOLON type TSEMI
    { $$ = Symboltable::defineUserType($2, $4); }
| TTYPE identifier TCOLON type TASSIGN expr TSEMI
    { $$ = Symboltable::defineUserType($2, $4, $6); }
;


enumdecl:
  TENUM identifier TASSIGN enumList TSEMI
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
    { $$ = nilSymbol; }
| TCOLON TYPE_IDENT
    {
      $$ = $2;   /* Symboltable::lookup($2); */
    }
;


classdecl:
  TCLASS identifier subclass TLCBR
    {
      $<ptsym>$ = Symboltable::startClassDef($2, $3);
    }
                                decls TRCBR
    {
      $$ = Symboltable::finishClassDef($<ptsym>5, $6);
    }
;


enumList:
  identsym
| enumList TBOR identsym
    {
      $1->append($3);
      $$ = $1;
    }
;


formaltag:
  /* nothing */
    { $$ = PARAM_INOUT; }
| TIN
    { $$ = PARAM_IN; }
| TINOUT
    { $$ = PARAM_INOUT; }
| TOUT
    { $$ = PARAM_OUT; }
| TCONST
    { $$ = PARAM_CONST; }
| TREF
    { $$ = PARAM_INOUT; }
| TVAL
    { $$ = PARAM_IN; }
;


formal:
  formaltag idlist vardecltype
    { $$ = Symboltable::defineParams($1, $2, $3); }
;


nonemptyformals:
  formal
| nonemptyformals TSEMI formal
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
| TCOLON type
    { $$ = $2; }
;


fndecl:
  TFUNCTION identifier
    {
      $<fnsym>$ = Symboltable::startFnDef($2);
    }
                       TLP formals TRP fnrettype statement
    {
      $$ = Symboltable::finishFnDef($<fnsym>3, $5, $7, $8);
    }
;


decl:
  vardecl
| typedecl
| fndecl
;


decls:
  /* empty */
    { $$ = nilStmt; }
| decls decl
    { $$ = appendLink($1, $2); }
;


tupleTypes:
  type
    { $$ = new TupleType($1); }
| tupleTypes TCOMMA type
    { 
      $1->addType($3);
      $$ = $1;
    }
;


tupleType:
  TLP tupleTypes TRP
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
  TDOMAIN
    { $$ = new DomainType(); }
| TDOMAIN TLP expr TRP
    { $$ = new DomainType($3); }
;


indexType:
  TINDEX
    { $$ = new IndexType(); }
| TINDEX TLP expr TRP
    { $$ = new IndexType($3); }
;


arrayType:
  TLSBR TRSBR type
    { $$ = new ArrayType(unknownDomain, $3); }
| TLSBR query_identifier TRSBR type
    { 
      Symboltable::defineQueryDomain($2);  // really need to tuck this into
                                           // a var def stmt to be inserted
                                           // as soon as the next stmt is
                                           // defined  -- BLC
      $$ = new ArrayType(unknownDomain, $4);
    }
| TLSBR domainExpr TRSBR type
    { $$ = new ArrayType($2, $4); }
;


statements:
  /* empty */
    { $$ = nilStmt; }
| statements statement
    { $$ = appendLink($1, $2); }
;


statement:
  TSEMI
    { $$ = new NoOpStmt(); }
| decl
| assignment
| conditional
| loop
| simple_lvalue TSEMI
    { $$ = new ExprStmt($1); }
| TCALL simple_lvalue TSEMI
    { $$ = new ExprStmt($2); }
| retStmt
| TLCBR
    { Symboltable::startCompoundStmt(); }
      statements TRCBR
    { $$ = Symboltable::finishCompoundStmt($3); }
| error
    { printf("syntax error"); exit(1); }
;


retStmt:
  TRETURN TSEMI
    { $$ = new ReturnStmt(nilExpr); }
| TRETURN expr TSEMI
    { $$ = new ReturnStmt($2); }
;


fortype:
  TFOR
| TFORALL
;


indexlist:
  idlist
| TLP idlist TRP
  { $$ = $2; }
;


forloop:
  fortype indexlist TIN expr
    { 
      $<pvsym>$ = Symboltable::startForLoop($2);
    }
                                 statement
    { 
      $$ = Symboltable::finishForLoop(true, $<pvsym>5, $4, $6);
    }
;


whileloop:
  TWHILE expr statement
    { $$ = new WhileLoopStmt(true, $2, $3); }
| TDO statement TWHILE expr TSEMI
    { $$ = new WhileLoopStmt(false, $4, $2); }
;


loop:
  forloop
| whileloop
;


conditional:
  TIF expr statement %prec TNOELSE
    { $$ = new CondStmt($2, $3); }
| TIF expr TTHEN statement %prec TNOELSE
    { $$ = new CondStmt($2, $4); }
| TIF expr statement TELSE statement
    { $$ = new CondStmt($2, $3, $5); }
| TIF expr TTHEN statement TELSE statement
    { $$ = new CondStmt($2, $4, $6); }
;


assignOp:
  TASSIGN
    { $$ = GETS_NORM; }
| TASSIGNPLUS
    { $$ = GETS_PLUS; }
| TASSIGNMINUS
    { $$ = GETS_MINUS; }
| TASSIGNMULTIPLY
    { $$ = GETS_MULT; }
| TASSIGNDIVIDE
    { $$ = GETS_DIV; }
| TASSIGNBAND
    { $$ = GETS_BITAND; }
| TASSIGNBOR
    { $$ = GETS_BITOR; }
| TASSIGNBXOR
    { $$ = GETS_BITXOR; }
;


assign_lvalue:
  lvalue
| TLSBR domainExpr TRSBR lvalue
  {
    $2->setForallExpr($4);
    $$ = $2;
  }
;


assignment:
  assign_lvalue assignOp expr TSEMI
    { $$ = new ExprStmt(new AssignOp($2, $1, $3)); }
;


exprlist:
  /* empty */
    { $$ = nilExpr; }
| nonemptyExprlist
;


nonemptyExprlist:
  expr
| nonemptyExprlist TCOMMA expr
    { $1->append($3); }
;


simple_lvalue:
  identifier
    { $$ = new Variable(Symboltable::lookup($1)); }
| simple_lvalue TDOT identifier
    { $$ = Symboltable::defineMemberAccess($1, $3); }
| simple_lvalue TLP exprlist TRP
    { $$ = ParenOpExpr::classify($1, $3); }
/*
| simple_lvalue TLSBR exprlist TRSBR
    { $$ = ParenOpExpr::classify($1, $3); }
*/
;


lvalue:
  simple_lvalue
;


atom:
  literal
| lvalue
;


expr: 
  atom
| TYPE_IDENT TLP exprlist TRP
{ $$ = ParenOpExpr::classify(new Variable($1), $3); }
| reduction %prec TREDUCE
| expr TCOLON type
    { $$ = new CastExpr($3, $1); }
| range %prec TDOTDOT
| TLP nonemptyExprlist TRP 
    { 
      if ($2->next->isNull()) {
        $$ = $2;
      } else {
        $$ = new Tuple($2);
      }
    }
| TLSBR domainExpr TRSBR expr
    {
      $2->setForallExpr($4);
      $$ = $2;
    }
| TPLUS expr %prec TUPLUS
    { $$ = new UnOp(UNOP_PLUS, $2); }
| TMINUS expr %prec TUMINUS
    { $$ = new UnOp(UNOP_MINUS, $2); }
| TNOT expr
    { $$ = new UnOp(UNOP_LOGNOT, $2); }
| TBNOT expr
    { $$ = new UnOp(UNOP_BITNOT, $2); }
| expr TPLUS expr
    { $$ = Expr::newPlusMinus(BINOP_PLUS, $1, $3); }
| expr TMINUS expr
    { $$ = Expr::newPlusMinus(BINOP_MINUS, $1, $3); }
| expr TSTAR expr
    { $$ = new BinOp(BINOP_MULT, $1, $3); }
| expr TDIVIDE expr
    { $$ = new BinOp(BINOP_DIV, $1, $3); }
| expr TMOD expr
    { $$ = new BinOp(BINOP_MOD, $1, $3); }
| expr TEQUAL expr
    { $$ = new BinOp(BINOP_EQUAL, $1, $3); }
| expr TNOTEQUAL expr
    { $$ = new BinOp(BINOP_NEQUAL, $1, $3); }
| expr TLESSEQUAL expr
    { $$ = new BinOp(BINOP_LEQUAL, $1, $3); }
| expr TGREATEREQUAL expr
    { $$ = new BinOp(BINOP_GEQUAL, $1, $3); }
| expr TLESS expr
    { $$ = new BinOp(BINOP_LTHAN, $1, $3); }
| expr TGREATER expr
    { $$ = new BinOp(BINOP_GTHAN, $1, $3); }
| expr TBAND expr
    { $$ = new BinOp(BINOP_BITAND, $1, $3); }
| expr TBOR expr
    { $$ = new BinOp(BINOP_BITOR, $1, $3); }
| expr TBXOR expr
    { $$ = new BinOp(BINOP_BITXOR, $1, $3); }
| expr TAND expr
    { $$ = new BinOp(BINOP_LOGAND, $1, $3); }
| expr TOR expr
    { $$ = new BinOp(BINOP_LOGOR, $1, $3); }
| expr TEXP expr
    { $$ = new BinOp(BINOP_EXP, $1, $3); }
| expr TBY expr
    { $$ = new SpecialBinOp(BINOP_BY, $1, $3); }
;


reduction:
  TYPE_IDENT TREDUCE expr
    { $$ = new ReduceExpr($1, nilExpr, $3); }
;


domainExpr:
  nonemptyExprlist
    { $$ = new DomainExpr($1); }
| nonemptyExprlist TIN nonemptyExprlist                             // BLC: This is wrong vv
    { $$ = new DomainExpr($3, Symboltable::defineVars(Symboltable::exprToIndexSymbols($1), dtInteger)); }
/*
  The above case replaces the following: (The first expressions must
  be checked to make sure they are simply identifiers, e.g., [i,j in D].)

| indexlist TIN nonemptyExprlist          // BLC: This is wrong vv
    { $$ = new DomainExpr($3, Symboltable::defineVars($1, dtInteger)); }
*/
;


range:
  expr TDOTDOT expr
    { $$ = new SimpleSeqExpr($1, $3); }
| TSTAR
    { $$ = new FloodExpr(); }
| TDOTDOT
    { $$ = new CompleteDimExpr(); }
;


literal:
  intliteral
    { $$ = new IntLiteral(yytext, $1); }
| FLOATLITERAL
    { $$ = new FloatLiteral(yytext, atof(yytext)); }
| COMPLEXLITERAL
    { $$ = new ComplexLiteral(yytext, atof(yytext)); }
| STRINGLITERAL
    { $$ = new StringLiteral($1); }
;


intliteral:
  INTLITERAL
    { $$ = atol(yytext); }
;


identifier:
  TIDENT
    { $$ = copystring(yytext); }
;


query_identifier:
  QUERY_IDENT
    { $$ = copystring(yytext+1); }
;


%%
