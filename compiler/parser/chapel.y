/* The CHAPEL Parser */

%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexyacc.h"

%}

%start program

%union	{
  bool boolval;
  char* pch;

  getsOpType got;
  varType vt;
  paramType pt;

  Expr* pexpr;
  ForallExpr* pfaexpr;
  Stmt* stmt;
  VarDefStmt* varstmt;
  ForLoopStmt* forstmt;
  BlockStmt* blkstmt;
  Type* pdt;
  TupleType* tupledt;
  EnumSymbol* enumsym;
  Symbol* psym;
  VarSymbol* pvsym;
  TypeSymbol* ptsym;
  FnSymbol* fnsym;
  ModuleSymbol* modsym;
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
%token TMODULE
%token TOUT
%token TRECORD
%token TREF
%token TRETURN
%token TSTATIC
%token TTHEN
%token TTYPE
%token TUNION
%token TVAL
%token TVAR
%token TWHILE
%token TWITH

%token TIDENT QUERY_IDENT
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

%type <got> assignOp
%type <vt> vardecltag
%type <pt> formaltag

%type <pdt> type domainType indexType arrayType tupleType
%type <tupledt> tupleTypes
%type <pdt> vardecltype fnrettype
%type <pch> identifier query_identifier
%type <psym> ident_symbol ident_symbol_ls formal formals indexes indexlist
%type <enumsym> enum_item enum_list
%type <pexpr> simple_lvalue lvalue atom expr exprlist nonemptyExprlist literal range
%type <pexpr> reduction optional_init_expr assignExpr
%type <pfaexpr> forallExpr
%type <stmt> program modulebody statements statement decls decl typevardecl
%type <varstmt> vardecl vardecl_inner vardecl_inner_ls
%type <stmt> assignment conditional retStmt loop forloop whileloop enumdecl 
%type <stmt> typealias typedecl fndecl classdecl recorddecl uniondecl moduledecl


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


program: modulebody
    { yystmtlist = $$; }
;

modulebody: 
  statements
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


ident_symbol:
  identifier
    { $$ = new Symbol(SYMBOL, $1); }
;


ident_symbol_ls:
  ident_symbol
| ident_symbol_ls ident_symbol
    {
      $1->append($2);
      $$ = $1;
    }
;


vardecltype:
  /* nothing */
    { $$ = dtUnknown; }
| TCOLON type
    { $$ = $2; }
;


optional_init_expr:
  /* nothing */
    { $$ = nilExpr; }
| TASSIGN expr
    { $$ = $2; }
;


vardecl_inner:
  ident_symbol_ls vardecltype optional_init_expr
    { $$ = Symboltable::defineVarDefStmt1($1, $2, $3); }
;


vardecl_inner_ls:
  vardecl_inner
| vardecl_inner_ls TCOMMA vardecl_inner
    {
      $1->append($3);
      $$ = $1;
    }
;


vardecl:
  vardecltag varconst vardecl_inner_ls TSEMI
    { $$ = Symboltable::defineVarDefStmt2($3, $1, $2); }
;


typedecl:
  typealias
| typevardecl
| enumdecl
| classdecl
| recorddecl
| uniondecl
;


typealias:
  TTYPE identifier TCOLON type optional_init_expr TSEMI
    {
      UserType* newtype = new UserType($4, $5);
      Symbol* typeSym = new TypeSymbol($2, newtype);
      newtype->addName(typeSym);
      $$ = new TypeDefStmt(newtype);
      typeSym->setDefPoint($$);
    }
;


typevardecl:
  TTYPE identifier TSEMI
    {
      VariableType* new_type = new VariableType();
      TypeSymbol* new_symbol = new TypeSymbol($2, new_type);
      new_type->addName(new_symbol);
      $$ = new TypeDefStmt(new_type);
      new_symbol->setDefPoint($$);
    }
;


enumdecl:
  TENUM identifier TLCBR enum_list TRCBR TSEMI
    {
      $4->set_values();
      EnumType* pdt = new EnumType($4);
      Symbol* pst = new TypeSymbol($2, pdt);
      pdt->addName(pst);
      $$ = new TypeDefStmt(pdt);
      pst->setDefPoint($$);
      $4->setDefPoint($$);
    }
;


classdecl:
  TCLASS identifier TLCBR
    {
      $<ptsym>$ = Symboltable::startClassDef($2, false, false);
    }
                                decls TRCBR
    {
      $$ = Symboltable::finishClassDef($<ptsym>4, $5);
    }
;


recorddecl:
  TRECORD identifier TLCBR
    {
      $<ptsym>$ = Symboltable::startClassDef($2, true, false);
    }
                                decls TRCBR
    {
      $$ = Symboltable::finishClassDef($<ptsym>4, $5);
    }
;


uniondecl:
  TUNION identifier TLCBR
    {
      $<ptsym>$ = Symboltable::startClassDef($2, false, true);
    }
                                decls TRCBR
    {
      $$ = Symboltable::finishClassDef($<ptsym>4, $5);
    }
;


enum_item:
  identifier
    {
      $$ = new EnumSymbol($1, nilExpr);
    }
| identifier TASSIGN expr
    {
      $$ = new EnumSymbol($1, $3);
    }
;


enum_list:
  enum_item
    {
      $$ = $1;
    }
| enum_list TCOMMA enum_item
    {
      $1->append($3);
      $$ = $1;
    }
;


formaltag:
  /* nothing */
    { $$ = PARAM_BLANK; }
| TIN
    { $$ = PARAM_IN; }
| TINOUT
    { $$ = PARAM_INOUT; }
| TOUT
    { $$ = PARAM_OUT; }
| TCONST
    { $$ = PARAM_CONST; }
;


formal:
  formaltag ident_symbol_ls vardecltype
    {
      $$ = Symboltable::defineParams($1, $2, $3);
      //      $$ = new ParamSymbol($1, $2, $3);
    }
| TTYPE identifier
    {
      VariableType* new_type = new VariableType();
      $$ = new TypeSymbol($2, new_type);
      new_type->addName($$);
    }
;


formals:
  /* empty */
    { $$ = nilSymbol; }
| formal
| formals TCOMMA formal
    {
      $1->append($3);
      $$ = $1;
    }
;


fnrettype:
  /* empty */
    { $$ = dtUnknown; }
| TCOLON type
    { $$ = $2; }
;


fndecl:
  TFUNCTION identifier
    {
      $<fnsym>$ = Symboltable::startFnDef(new FnSymbol($2));
    }
                       TLP formals TRP fnrettype statement
    {
      $$ = Symboltable::finishFnDef($<fnsym>3, $5, $7, $8);
    }
;


moduledecl:
  TMODULE identifier
    {
      $<modsym>$ = Symboltable::startModuleDef($2);
    }
                     TLCBR modulebody TRCBR
    {
      $$ = Symboltable::finishModuleDef($<modsym>3, $5);
    }
;


decl:
  TWITH simple_lvalue TSEMI
    { $$ = new WithStmt($2); }
| vardecl
    { $$ = $1; }
| typedecl
| fndecl
| moduledecl
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
| identifier
    { $$ = new UnresolvedType($1); }
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


forallExpr:
  TLSBR nonemptyExprlist TRSBR
    { $$ = Symboltable::startForallExpr($2); }
| TLSBR nonemptyExprlist TIN nonemptyExprlist TRSBR
    { $$ = Symboltable::startForallExpr($4, $2); }
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
| forallExpr type
    {
      Symboltable::finishForallExpr($1);
      $$ = new ArrayType($1, $2);
    }
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
    { $<blkstmt>$ = Symboltable::startCompoundStmt(); }
      statements TRCBR
    { $$ = Symboltable::finishCompoundStmt($<blkstmt>2, $3); }
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


indexes:
  ident_symbol
| indexes TCOMMA ident_symbol
    {
      $1->append($3);
      $$ = $1;
    }
;


indexlist:
  indexes
| TLP indexes TRP
  { $$ = $2; }
;


forloop:
  fortype indexlist TIN expr
    { 
      $<forstmt>$ = Symboltable::startForLoop(true, $2, $4);
    }
                                 statement
    { 
      $$ = Symboltable::finishForLoop($<forstmt>5, $6);
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


assignExpr:
  lvalue assignOp expr
    { $$ = new AssignOp($2, $1, $3); }
| forallExpr assignExpr
    { $$ = Symboltable::finishForallExpr($1, $2); }
;


assignment:
  assignExpr TSEMI
    { $$ = new ExprStmt($1); }
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
    { $$ = new Variable(new UnresolvedSymbol($1)); }
| simple_lvalue TDOT identifier
    { $$ = new MemberAccess($1, new UnresolvedSymbol($3)); }
| simple_lvalue TLP exprlist TRP
    { $$ = new ParenOpExpr($1, $3); }
/*
| simple_lvalue TLSBR exprlist TRSBR
    { $$ = new ParenOpExpr($1, $3); }
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
| forallExpr expr %prec TRSBR
    { $$ = Symboltable::finishForallExpr($1, $2); }
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
  identifier TREDUCE expr
    { $$ = new ReduceExpr(new UnresolvedSymbol($1), nilExpr, $3); }
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
  INTLITERAL
    { $$ = new IntLiteral(yytext, atol(yytext)); }
| FLOATLITERAL
    { $$ = new FloatLiteral(yytext, atof(yytext)); }
| COMPLEXLITERAL
    { $$ = new ComplexLiteral(yytext, atof(yytext)); }
| STRINGLITERAL
    { $$ = new StringLiteral($1); }
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
