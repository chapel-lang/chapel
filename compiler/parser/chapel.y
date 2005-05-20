/* The CHAPEL Parser */

%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexyacc.h"
#include "chplalloc.h"

%}

%start program

%union  {
  bool boolval;
  char* pch;

  getsOpType got;
  varType vt;
  consType ct;
  paramType pt;

  Expr* pexpr;
  ForallExpr* pfaexpr;
  Stmt* stmt;
  DefStmt* defstmt;
  DefExpr* defexpr;
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
  Pragma *pragmas;
}

%token TBREAK
%token TCALL
%token TCLASS
%token TCONFIG
%token TCONST
%token TPARAM
%token TCONTINUE
%token TDO
%token TDOMAIN
%token TENUM
%token TFOR
%token TFORALL
%token TFUNCTION
%token TGOTO
%token TIF
%token TIN
%token TINDEX
%token TINOUT
%token TLABEL
%token TLET
%token TLIKE
%token TMODULE
%token TNIL
%token TOF
%token TOUT
%token TPRAGMA
%token TRECORD
%token TREF
%token TRETURN
%token TSEQ
%token TSTATIC
%token TTHEN
%token TTYPE
%token TUNION
%token TUSE
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
%token TSEQBEGIN;
%token TSEQEND;
%token TLSBR;
%token TRSBR;
%token TLCBR;
%token TRCBR;


%token TQUESTION;

%type <ct> varconst
 
%type <got> assignOp
%type <vt> vardecltag
%type <pt> formaltag

%type <boolval> fortype fnretref
%type <pdt> type domainType indexType arrayType tupleType seqType
%type <tupledt> tupleTypes
%type <pdt> vardecltype typevardecltype fnrettype
%type <pch> identifier query_identifier fname
%type <psym> ident_symbol ident_symbol_ls formal formals indexes indexlist
%type <enumsym> enum_item enum_list
%type <pexpr> lvalue declarable_expr atom expr exprlist expr_list_item nonemptyExprlist literal range seq_expr
%type <pexpr> reduction optional_init_expr assignExpr
%type <pfaexpr> forallExpr
%type <stmt> program modulebody statements statement call_stmt noop_stmt decls decl typevardecl
%type <defexpr> vardecl_inner vardecl_inner_ls
%type <defstmt> vardecl
%type <stmt> assignment conditional retStmt loop forloop whileloop enumdecl
%type <pdt> structtype
%type <stmt> typealias typedecl fndecl structdecl moduledecl
%type <stmt> function_body_single_stmt function_body_stmt block_stmt
%type <pragmas> pragma pragmas


/* These are declared in increasing order of precedence. */

%left TNOELSE
%left TELSE

%left TRSBR
%left TIN
%left TBY
%left TDOTDOT
%left TSEQCAT
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
    { $$ = VAR_VAR; }
| TCONST
    { $$ = VAR_CONST; }
| TPARAM
    { $$ = VAR_PARAM; }
;
        
ident_symbol:
   pragma ident_symbol
    { 
      $$ = $2; 
      $1->next = $$->pragmas;
      $$->pragmas = $1;
    } 
|  identifier
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
| TLIKE expr
    { $$ = new LikeType($2); }
;


optional_init_expr:
  /* nothing */
    { $$ = NULL; }
| TASSIGN expr
    { $$ = $2; }
;


vardecl_inner:
  ident_symbol_ls vardecltype optional_init_expr
    { $$ = Symboltable::defineVarDef1($1, $2, $3); }
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
    { $$ = new DefStmt(Symboltable::defineVarDef2($3, $1, $2)); }
;

typedecl:
  typealias
| typevardecl
| enumdecl
| structdecl
;


typealias:
  TTYPE pragmas identifier TCOLON type optional_init_expr TSEMI
    {
      UserType* newtype = new UserType($5, $6);
      TypeSymbol* typeSym = new TypeSymbol($3, newtype);
      typeSym->pragmas = $2;
      newtype->addSymbol(typeSym);
      DefExpr* def_expr = new DefExpr(typeSym);
      $$ = new DefStmt(def_expr);
    }
;


typevardecl:
  TTYPE pragmas identifier TSEMI
    {
      VariableType* new_type = new VariableType(getMetaType(0));
      TypeSymbol* new_symbol = new TypeSymbol($3, new_type);
      new_symbol->pragmas = $2;
      new_type->addSymbol(new_symbol);
      DefExpr* def_expr = new DefExpr(new_symbol);
      $$ = new DefStmt(def_expr);
    }
;


enumdecl:
  TENUM pragmas identifier TLCBR enum_list TRCBR TSEMI
    {
      $5->set_values();
      EnumType* pdt = new EnumType($5);
      TypeSymbol* pst = new TypeSymbol($3, pdt);
      pst->pragmas = $2;
      pdt->addSymbol(pst);
      DefExpr* def_expr = new DefExpr(pst);
      $5->setDefPoint(def_expr);  /* SJD: Should enums have more DefExprs? */
      $$ = new DefStmt(def_expr);
    }
;


structtype:
  TCLASS
    { $$ = new ClassType(); }
| TRECORD
    { $$ = new RecordType(); }
| TUNION
    { $$ = new UnionType(); }
;


structdecl:
  structtype pragmas identifier TLCBR
    {
      $<ptsym>$ = Symboltable::startStructDef($1, $3);
      $<ptsym>$->pragmas = $2;
    }
                                      decls TRCBR
    {
      $$ = new DefStmt(Symboltable::finishStructDef($<ptsym>5, $6));
    }
;


enum_item:
  identifier
    {
      $$ = new EnumSymbol($1, NULL);
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


typevardecltype:
  /* nothing */
    { $$ = NULL; }
| TCOLON type
    { $$ = $2; }
;


formal:
  formaltag ident_symbol_ls vardecltype optional_init_expr
    {
      $$ = Symboltable::defineParams($1, $2, $3, $4);
    }
| TTYPE ident_symbol typevardecltype
    {
      ParamSymbol *ps = Symboltable::defineParams(PARAM_BLANK, $2, getMetaType($3), NULL);
      char *name = glomstrings(2, "__type_variable_", ps->name);
      VariableType* new_type = new VariableType(getMetaType($3));
      TypeSymbol* new_type_symbol = new TypeSymbol(name, new_type);
      new_type->addSymbol(new_type_symbol);
      ps->typeVariable = new_type_symbol;
      $$ = ps;
    }
;


formals:
  /* empty */
    { $$ = NULL; }
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


fnretref:
  /* empty */
    { $$ = false; }
| TVAR
    { $$ = true; }
;


fname:
  identifier
| TASSIGN 
  { $$ = "="; } 
| TASSIGNPLUS
  { $$ = "+="; } 
| TASSIGNMINUS
  { $$ = "-="; } 
| TASSIGNMULTIPLY
  { $$ = "*="; } 
| TASSIGNDIVIDE
  { $$ = "/="; } 
| TASSIGNBAND
  { $$ = "&="; } 
| TASSIGNBOR
  { $$ = "|="; } 
| TASSIGNBXOR
  { $$ = "^="; } 
| TBAND
  { $$ = "&"; } 
| TBOR
  { $$ = "|"; } 
| TBXOR
  { $$ = "^"; } 
| TBNOT
  { $$ = "~"; } 
| TEQUAL
  { $$ = "=="; } 
| TNOTEQUAL
  { $$ = "!="; } 
| TLESSEQUAL
  { $$ = "<="; } 
| TGREATEREQUAL
  { $$ = ">="; } 
| TLESS
  { $$ = "<"; } 
| TGREATER
  { $$ = ">"; } 
| TPLUS 
  { $$ = "+"; } 
| TMINUS
  { $$ = "-"; } 
| TSTAR
  { $$ = "*"; } 
| TDIVIDE
  { $$ = "/"; } 
| TMOD
  { $$ = "mod"; } 
| TEXP
  { $$ = "**"; } 
| TAND
  { $$ = "and"; } 
| TOR
  { $$ = "or"; } 
| TBY
  { $$ = "by"; } 
| TSEQCAT
  { $$ = "#"; } 
  ;

fndecl:
  TFUNCTION fname
    {
      $<fnsym>$ = Symboltable::startFnDef(new FnSymbol($2));
    }
                       TLP formals TRP fnretref fnrettype
    {
      Symboltable::continueFnDef($<fnsym>3, $5, $8, $7);
    }
                                                 function_body_stmt
    {
      $$ = new DefStmt(new DefExpr(Symboltable::finishFnDef($<fnsym>3, $10)));
    }
|
  TFUNCTION identifier TDOT fname
    {
      $<fnsym>$ =
        Symboltable::startFnDef(new FnSymbol($4, new UnresolvedSymbol($2)));
    }
                                  TLP formals TRP fnretref fnrettype
    {
      Symboltable::continueFnDef($<fnsym>5, $7, $10, $9);
    }
                                                            function_body_stmt
    {
      $$ = new DefStmt(new DefExpr(Symboltable::finishFnDef($<fnsym>5, $12)));
    }
|
  TFUNCTION fname
    {
      $<fnsym>$ = Symboltable::startFnDef(new FnSymbol($2), true);
    }
                  fnretref fnrettype
    {
      Symboltable::continueFnDef($<fnsym>3, NULL, $5, $4);
    }
                            function_body_stmt
    {
      $$ = new DefStmt(new DefExpr(Symboltable::finishFnDef($<fnsym>3, $7)));
    }
;


moduledecl:
  TMODULE identifier
    {
      $<modsym>$ = Symboltable::startModuleDef($2);
    }
                     TLCBR modulebody TRCBR
    {
      $$ = new DefStmt(Symboltable::finishModuleDef($<modsym>3, $5));
    }
;


decl:
  TWITH lvalue TSEMI
    { $$ = new WithStmt($2); }
| TUSE lvalue TSEMI
    { $$ = new UseStmt($2); }
| vardecl
    { $$ = $1; }
| typedecl
| fndecl
| moduledecl
;


decls:
  /* empty */
    { $$ = NULL; }
| decls pragmas decl
    {
      $$ = appendLink($1, $3); 
      $3->pragmas = $2;
    }
;


tupleTypes:
  type
    {
      $$ = new TupleType();
      $$->addType($1);
    }
| tupleTypes TCOMMA type
    { 
      $$ = $1;
      $$->addType($3);
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
| seqType
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


seqType:
  TSEQ TOF type
    { $$ = new SeqType($3); }
| TSEQ TLP type TRP
    { $$ = new SeqType($3); }
;


statements:
  /* empty */
    { $$ = NULL; }
| statements pragmas statement
    { 
      $$ = appendLink($1, $3); 
      $3->pragmas = $2;
    }
;


function_body_single_stmt:
  noop_stmt
| conditional
| loop
| call_stmt
| retStmt
;


function_body_stmt:
  function_body_single_stmt
| block_stmt
;


statement:
  noop_stmt
| TLABEL identifier statement
{ $$ = new LabelStmt(new LabelSymbol($2), new BlockStmt($3)); }
| TGOTO identifier TSEMI
    { $$ = new GotoStmt(goto_normal, $2); }
| TBREAK identifier TSEMI
    { $$ = new GotoStmt(goto_break, $2); }
| TBREAK TSEMI
    { $$ = new GotoStmt(goto_break); }
| TCONTINUE identifier TSEMI
    { $$ = new GotoStmt(goto_continue, $2); }
| TCONTINUE TSEMI
    { $$ = new GotoStmt(goto_continue); }
| decl
| assignment
| conditional
| loop
| call_stmt
| lvalue TSEMI
    { $$ = new ExprStmt($1); }
| retStmt
| block_stmt
    { $$ = $1; }
| error
    { printf("syntax error"); exit(1); }
;


pragmas:
  /* empty */
    { $$ = NULL; }
| pragmas pragma
    { $$ = appendLink($1, $2); }
;


pragma:
  TPRAGMA STRINGLITERAL
  { $$ = new Pragma($2); }
;


call_stmt:
  TCALL lvalue TSEMI
    { $$ = new ExprStmt($2); }
;


noop_stmt:
  TSEMI
    { $$ = new NoOpStmt(); }
;


block_stmt:
  TLCBR
    { $<blkstmt>$ = Symboltable::startCompoundStmt(); }
        statements TRCBR
    { $$ = Symboltable::finishCompoundStmt($<blkstmt>2, $3); }
;


retStmt:
  TRETURN TSEMI
    { $$ = new ReturnStmt(NULL); }
| TRETURN expr TSEMI
    { $$ = new ReturnStmt($2); }
;


fortype:
  TFOR
    { $$ = false; }
| TFORALL
    { $$ = true; }
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
      $<forstmt>$ = Symboltable::startForLoop($1, $2, $4);
    }
                             block_stmt
    { 
      $$ = Symboltable::finishForLoop($<forstmt>5, $6);
    }
| fortype indexlist TIN expr
    { 
      $<forstmt>$ = Symboltable::startForLoop($1, $2, $4);
    }
                             TDO statement
    { 
      $$ = Symboltable::finishForLoop($<forstmt>5, $7);
    }
| TLSBR indexlist TIN expr TRSBR
    { 
      $<forstmt>$ = Symboltable::startForLoop(true, $2, $4);
    }
                                 statement
    { 
      $$ = Symboltable::finishForLoop($<forstmt>6, $7);
    }
;


whileloop:
TWHILE expr TDO statement
    { $$ = new WhileLoopStmt(true, $2, $4); }
| TWHILE expr block_stmt
    { $$ = new WhileLoopStmt(true, $2, $3); }
| TDO statement TWHILE expr TSEMI
    { $$ = new WhileLoopStmt(false, $4, $2); }
;


loop:
  forloop
| whileloop
;


conditional:
  TIF expr block_stmt %prec TNOELSE
    { $$ = new CondStmt($2, dynamic_cast<BlockStmt*>($3)); }
| TIF expr TTHEN statement %prec TNOELSE
    { $$ = new CondStmt($2, new BlockStmt($4)); }
| TIF expr block_stmt TELSE statement
    { $$ = new CondStmt($2, dynamic_cast<BlockStmt*>($3), new BlockStmt($5)); }
| TIF expr TTHEN statement TELSE statement
    { $$ = new CondStmt($2, new BlockStmt($4), new BlockStmt($6)); }
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
;


assignment:
  assignExpr TSEMI
    { $$ = new ExprStmt($1); }
;


exprlist:
  /* empty */
    { $$ = NULL; }
| nonemptyExprlist
;


expr_list_item:
  identifier TASSIGN expr
    { $$ = new NamedExpr($1, $3); }
| expr
    { $$ = $1; }
;


nonemptyExprlist:
  pragmas expr_list_item
    { $2->pragmas = $1; $$ = $2; }
| nonemptyExprlist TCOMMA pragmas expr_list_item
    { $4->pragmas = $3; $1->append($4); }
;


declarable_expr:
  identifier
    { $$ = new Variable(new UnresolvedSymbol($1)); }
| TLP nonemptyExprlist TRP 
    { 
      if (!$2->next) {
        $$ = $2;
      } else {
        $$ = new Tuple($2);
      }
    }
;

lvalue:
  declarable_expr
| lvalue TDOT identifier
    { $$ = new MemberAccess($1, new UnresolvedSymbol($3)); }
| lvalue TLP exprlist TRP
    { $$ = new ParenOpExpr($1, $3); }
;


atom:
  literal
| lvalue
;


seq_expr:
  TSEQBEGIN exprlist TSEQEND
    { $$ = new SeqExpr($2); }
;


expr: 
  atom
| TNIL
    { $$ = new Variable(Symboltable::lookupInternal("nil", SCOPE_INTRINSIC)); }
| TLET
    { $<pexpr>$ = Symboltable::startLetExpr(); }
       vardecl_inner_ls TIN expr
    { $$ = Symboltable::finishLetExpr($<pexpr>2, $3, $5); }
| reduction %prec TREDUCE
| expr TCOLON type
  { $$ = new CastExpr($3, $1); }
| expr TCOLON STRINGLITERAL
  { 
    Variable* _chpl_tostring =
      new Variable(new UnresolvedSymbol("_chpl_tostring"));
    Expr* args = $1;
    args->append(new StringLiteral($3));
    $$ = new ParenOpExpr(_chpl_tostring, args);
  }
| range %prec TDOTDOT
| seq_expr
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
| expr TSEQCAT expr
    { $$ = new BinOp(BINOP_SEQCAT, $1, $3); }
| expr TBY expr
    { $$ = new SpecialBinOp(BINOP_BY, $1, $3); }
;

reduction:
  identifier TREDUCE expr
    { $$ = new ReduceExpr(new UnresolvedSymbol($1), NULL, $3); }
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
