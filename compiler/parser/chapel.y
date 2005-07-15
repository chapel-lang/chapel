%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexyacc.h" // all #includes here, for make depend

%}

%start program

%union  {
  bool boolval;
  char* pch;
  Vec<char*>* vpch;

  getsOpType got;
  varType vt;
  consType ct;
  paramType pt;
  blockStmtType blktype;
  fnType ft;

  Expr* pexpr;
  DefExpr* pdefexpr;
  ForallExpr* pforallexpr;

  Stmt* pstmt;
  WhenStmt* pwhenstmt;
  ForLoopStmt* pforloopstmt;
  BlockStmt* pblockstmt;

  Type* ptype;

  Symbol* psym;
  FnSymbol* pfnsym;
  ModuleSymbol* pmodsym;
  EnumSymbol* penumsym;

  AList<Expr>* pexprls;
  AList<Stmt>* pstmtls;
  AList<WhenStmt>* pwhenstmtls;
  AList<DefExpr>* pdefexprls;
  AList<EnumSymbol>* penumsymls;
  AList<Symbol>* psymls;
}

%token TATOMIC
%token TBREAK
%token TCALL
%token TCLASS
%token TCOBEGIN
%token TCONFIG
%token TCONST
%token TCONSTRUCTOR
%token TPARAMETER
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
%token TITERATOR
%token TLABEL
%token TLET
%token TLIKE
%token TMODULE
%token TNIL
%token TOF
%token TOTHERWISE
%token TOUT
%token TPRAGMA
%token TRECORD
%token TREF
%token TRETURN
%token TSELECT
%token TSTATIC
%token TTHEN
%token TTYPE
%token TUNION
%token TUSE
%token TVAL
%token TVAR
%token TWHEN
%token TWHERE
%token TWHILE
%token TWITH
%token TYIELD

%token TIDENT
%token INTLITERAL FLOATLITERAL COMPLEXLITERAL
%token <pch> STRINGLITERAL

%token TASSIGN
%token TASSIGNPLUS
%token TASSIGNMINUS
%token TASSIGNMULTIPLY
%token TASSIGNDIVIDE
%token TASSIGNBAND
%token TASSIGNBOR
%token TASSIGNBXOR
%token TASSIGNSEQCAT

%token TSEMI
%token TCOMMA
%token TDOT
%token TLP
%token TRP
%token TSEQBEGIN
%token TSEQEND
%token TLSBR
%token TRSBR
%token TLCBR
%token TRCBR
%token TCOLON
%token TNOTCOLON


%token TQUESTION

%type <vt> config_static
%type <ct> var_const_param
 
%type <got> assign_op_tag
%type <pt> formal_tag
%type <ft> fn_tag
%type <blktype> atomic_cobegin

%type <boolval> fortype fnretref

%type <pch> identifier fname opt_identifier
%type <pch> pragma
%type <vpch> pragma_ls

%type <pexpr> parenop_expr memberaccess_expr non_tuple_lvalue lvalue
%type <pexpr> tuple_paren_expr atom expr expr_list_item
%type <pexpr> literal range seq_expr where whereexpr
%type <pexpr> tuple_multiplier intliteral
%type <pexpr> reduction opt_init_expr assign_expr if_expr
%type <pexprls> expr_ls nonempty_expr_ls tuple_inner_type_ls
%type <pdefexpr> formal
%type <pdefexprls> formal_ls opt_formal_ls
%type <pforallexpr> forallExpr

%type <pstmt> select_stmt
%type <pstmt> usertype_decl type_decl fn_decl struct_decl mod_decl
%type <pstmt> function_body_single_stmt 
%type <pstmt> assign_stmt if_stmt return_stmt loop forloop whileloop enumdecl
%type <pstmt> statement call_stmt noop_stmt decl typevar_decl
%type <pstmtls> decl_ls statements modulebody program
%type <pstmtls> var_decl var_decl_inner var_decl_inner_ls record_inner_var_ls
%type <pblockstmt> function_body_stmt block_stmt
%type <pwhenstmt> when_stmt
%type <pwhenstmtls> when_stmts

%type <ptype> opt_var_type var_type typevar_type fnrettype
%type <ptype> type domain_type index_type array_type record_tuple_type
%type <ptype> record_tuple_inner_type expr_type
%type <ptype> class_record_union

%type <psym> ident_symbol
%type <psymls> indexes indexlist
%type <pfnsym> function
%type <penumsym> enum_item
%type <penumsymls> enum_list

/* These are declared in increasing order of precedence. */


%left TNOELSE
%left TELSE

%left TCOMMA
%left TBY

%left TCOLON
%left TNOTCOLON

%left TRSBR
%left TIN
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
%right TUPLUS TUMINUS TREDUCE TSCAN TBNOT
%right TEXP

%left TLP
%left TDOT

%% 


program: modulebody
    { yystmtlist = $$; }
;


modulebody: 
  statements
;

config_static:
  /* nothing */
    { $$ = VAR_NORMAL; }
| TCONFIG
    { $$ = VAR_CONFIG; }
| TSTATIC
    { $$ = VAR_STATE; }
;


var_const_param:
  TVAR
    { $$ = VAR_VAR; }
| TCONST
    { $$ = VAR_CONST; }
| TPARAMETER
    { $$ = VAR_PARAM; }
;
        
ident_symbol:
  pragma_ls identifier
    { 
      $$ = new Symbol(SYMBOL, $2);
      $$->copyPragmas(*$1);
    } 
;


var_type:
  TCOLON type
    { $$ = $2; }
| TLIKE expr
    { $$ = new ExprType(new ParenOpExpr(new Variable(new UnresolvedSymbol("typeof")), new AList<Expr>($2))); }
;


opt_var_type:
  /* nothing */
    { $$ = dtUnknown; }
| var_type
;


opt_init_expr:
  /* nothing */
    { $$ = NULL; }
| TASSIGN expr
    { $$ = $2; }
;


var_decl_inner:
  identifier opt_var_type opt_init_expr
    {
      $$ = Symboltable::defineVarDef1($1, $2, $3);
    }
;


var_decl_inner_ls:
  var_decl_inner
| var_decl_inner_ls TCOMMA var_decl_inner
    {
      $1->add($3);
      $$ = $1;
    }
;


var_decl:
  config_static var_const_param var_decl_inner_ls TSEMI
    {
      Symboltable::defineVarDef2($3, $1, $2);
      $$ = $3;
    }
;

type_decl:
  usertype_decl
| typevar_decl
| enumdecl
| struct_decl
;


usertype_decl:
  TTYPE pragma_ls identifier TCOLON type opt_init_expr TSEMI
    {
      UserType* newtype = new UserType($5, $6);
      TypeSymbol* typeSym = new TypeSymbol($3, newtype);
      typeSym->copyPragmas(*$2);
      newtype->addSymbol(typeSym);
      DefExpr* def_expr = new DefExpr(typeSym);
      $$ = new ExprStmt(def_expr);
    }
;


typevar_decl:
  TTYPE pragma_ls identifier TSEMI
    {
      VariableType* new_type = new VariableType(getMetaType(0));
      TypeSymbol* new_symbol = new TypeSymbol($3, new_type);
      new_symbol->copyPragmas(*$2);
      new_type->addSymbol(new_symbol);
      DefExpr* def_expr = new DefExpr(new_symbol);
      $$ = new ExprStmt(def_expr);
    }
;


enumdecl:
  TENUM pragma_ls identifier TLCBR enum_list TRCBR TSEMI
    {
      EnumSymbol::setValues($5);
      EnumType* pdt = new EnumType($5);
      TypeSymbol* pst = new TypeSymbol($3, pdt);
      pst->copyPragmas(*$2);
      pdt->addSymbol(pst);
      DefExpr* def_expr = new DefExpr(pst);
      Symbol::setDefPoints($5, def_expr);
        /* SJD: Should enums have more DefExprs? */
      $$ = new ExprStmt(def_expr);
    }
;


class_record_union:
  TCLASS
    { $$ = new ClassType(); }
| TRECORD
    { $$ = new RecordType(); }
| TUNION
    { $$ = new UnionType(); }
;


struct_decl:
  class_record_union pragma_ls identifier TLCBR
    {
      Symboltable::pushScope(SCOPE_CLASS);
    }
                                                decl_ls TRCBR
    {
      SymScope *scope = Symboltable::popScope();
      Type* type = Symboltable::defineStructType($3, $1, scope, $6);
      type->symbol->copyPragmas(*$2);
      $$ = new ExprStmt(type->symbol->defPoint);
    }
;


tuple_inner_type_ls:
  lvalue
    {
      $$ = new AList<Expr>($1);
    }
| tuple_inner_type_ls TCOMMA lvalue
    { 
      $1->insertAtTail($3);
      $$ = $1;
    }
;


record_inner_var_ls:
  identifier var_type opt_init_expr
    {
      $$ = Symboltable::defineVarDef1($1, $2, $3);
    }
| record_inner_var_ls TCOMMA identifier var_type
    {
      $1->add(Symboltable::defineVarDef1($3, $4, NULL));
      $$ = $1;
    }
;


record_tuple_inner_type:
  record_inner_var_ls TRP
    {
      SymScope *scope = Symboltable::popScope();
      $$ = Symboltable::defineStructType(NULL, new RecordType(), scope, $1);
    }
| tuple_inner_type_ls TRP
    {
      Symboltable::popScope();
      char *tupleName = glomstrings(2, "_tuple", intstring($1->length()));
      $$ = new ExprType(
             new ParenOpExpr(
               new Variable(new UnresolvedSymbol(tupleName)), $1));
    }
;


record_tuple_type:
  TRECORD TLCBR
    {
      Symboltable::pushScope(SCOPE_CLASS);
    }
                decl_ls TRCBR
    {
      SymScope *scope = Symboltable::popScope();
      $$ = Symboltable::defineStructType(NULL, new RecordType(), scope, $4);
    }
| TLP
    {
      Symboltable::pushScope(SCOPE_CLASS);
    }
      record_tuple_inner_type
    {
      $$ = $3;
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
      $$ = new AList<EnumSymbol>($1);
    }
| enum_list TCOMMA enum_item
    {
      $1->insertAtTail($3);
      $$ = $1;
    }
;


formal_tag:
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
| TPARAMETER
    { $$ = PARAM_PARAMETER; }
;


typevar_type:
  /* nothing */
    { $$ = NULL; }
| TCOLON type
    { $$ = $2; }
;


formal:
  formal_tag pragma_ls identifier opt_var_type opt_init_expr
    {
      $$ = Symboltable::defineParam($1, $3, $4, $5);
      $$->sym->copyPragmas(*$2);
    }
| TTYPE pragma_ls identifier typevar_type
    {
      DefExpr* defExpr = Symboltable::defineParam(PARAM_TYPE, $3, getMetaType($4), NULL);
      defExpr->sym->copyPragmas(*$2);
      ParamSymbol* ps = dynamic_cast<ParamSymbol*>(defExpr->sym);
      char *name = glomstrings(2, "__type_variable_", ps->name);
      VariableType* new_type = new VariableType(getMetaType($4));
      TypeSymbol* new_type_symbol = new TypeSymbol(name, new_type);
      new_type->addSymbol(new_type_symbol);
      ps->typeVariable = new_type_symbol;
      $$ = defExpr;
    }
;


formal_ls:
  /* empty */
    {
      $$ = new AList<DefExpr>();
    }
| formal
    {
      $$ = new AList<DefExpr>($1);
    }
| formal_ls TCOMMA formal
    { 
      $1->insertAtTail($3);
      $$ = $1;
    }
;


opt_formal_ls:
  { $$ = NULL; }
| TLP formal_ls TRP
  { $$ = $2; }
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
| TASSIGN identifier
  { $$ = glomstrings(2, "=", $2); } 
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
| TASSIGNSEQCAT
  { $$ = "#="; }
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
| TSEQCAT
  { $$ = "#"; } 
| TBY
  { $$ = "by"; } 
  ;

fn_tag:
  TFUNCTION
    { $$ = FN_FUNCTION; }
| TCONSTRUCTOR
    { $$ = FN_CONSTRUCTOR; }
| TITERATOR
    { $$ = FN_ITERATOR; }
;

where:
  /* empty */
    { $$ = NULL; }
| TWHERE whereexpr
    { $$ = $2; }
;

whereexpr: 
  atom
| TTYPE identifier
    { $$ = new DefExpr(new TypeSymbol($2, new VariableType)); }
| TNOT whereexpr
    { $$ = new UnOp(UNOP_LOGNOT, $2); }
| TBNOT whereexpr
    { $$ = new UnOp(UNOP_BITNOT, $2); }
| whereexpr TPLUS whereexpr
    { $$ = Expr::newPlusMinus(BINOP_PLUS, $1, $3); }
| whereexpr TMINUS whereexpr
    { $$ = Expr::newPlusMinus(BINOP_MINUS, $1, $3); }
| whereexpr TSTAR whereexpr
    { $$ = new BinOp(BINOP_MULT, $1, $3); }
| whereexpr TDIVIDE whereexpr
    { $$ = new BinOp(BINOP_DIV, $1, $3); }
| whereexpr TMOD whereexpr
    { $$ = new BinOp(BINOP_MOD, $1, $3); }
| whereexpr TEQUAL whereexpr
    { $$ = new BinOp(BINOP_EQUAL, $1, $3); }
| whereexpr TNOTEQUAL whereexpr
    { $$ = new BinOp(BINOP_NEQUAL, $1, $3); }
| whereexpr TLESSEQUAL whereexpr
    { $$ = new BinOp(BINOP_LEQUAL, $1, $3); }
| whereexpr TGREATEREQUAL whereexpr
    { $$ = new BinOp(BINOP_GEQUAL, $1, $3); }
| whereexpr TLESS whereexpr
    { $$ = new BinOp(BINOP_LTHAN, $1, $3); }
| whereexpr TGREATER whereexpr
    { $$ = new BinOp(BINOP_GTHAN, $1, $3); }
| whereexpr TBAND whereexpr
    { $$ = new BinOp(BINOP_BITAND, $1, $3); }
| whereexpr TBOR whereexpr
    { $$ = new BinOp(BINOP_BITOR, $1, $3); }
| whereexpr TBXOR whereexpr
    { $$ = new BinOp(BINOP_BITXOR, $1, $3); }
| whereexpr TAND whereexpr
    { $$ = new BinOp(BINOP_LOGAND, $1, $3); }
| whereexpr TCOMMA whereexpr
    { $$ = new BinOp(BINOP_LOGAND, $1, $3); }
| whereexpr TOR whereexpr
    { $$ = new BinOp(BINOP_LOGOR, $1, $3); }
| whereexpr TEXP whereexpr
    { $$ = new BinOp(BINOP_EXP, $1, $3); }
| whereexpr TCOLON whereexpr
    { $$ = new BinOp(BINOP_SUBTYPE, $1, $3); }
| whereexpr TNOTCOLON whereexpr
    { $$ = new BinOp(BINOP_NOTSUBTYPE, $1, $3); }
| class_record_union pragma_ls opt_identifier TLCBR decl_ls TRCBR
    { $$ = NULL; }
;


function:
  fname
    { $$ = new FnSymbol($1); }
| identifier TDOT fname
    { $$ = new FnSymbol($3, new UnresolvedSymbol($1)); }
;


fn_decl:
  fn_tag function
    {
      $<pfnsym>$ = Symboltable::startFnDef($2);
      $<pfnsym>$->fnClass = $1;
    }
                  opt_formal_ls fnretref fnrettype where
    {
      if (!$4) {
        $4 = new AList<DefExpr>();
        $<pfnsym>3->noparens = true;
      }
      Symboltable::continueFnDef($<pfnsym>3, $4, $6, $5, $7);
    }
                                                         function_body_stmt
    {
      $$ = new ExprStmt(new DefExpr(Symboltable::finishFnDef($<pfnsym>3, $9)));
    }
;


mod_decl:
  TMODULE identifier
    {
      $<pmodsym>$ = Symboltable::startModuleDef($2);
    }
                     TLCBR modulebody TRCBR
    {
      $$ = new ExprStmt(Symboltable::finishModuleDef($<pmodsym>3, $5));
    }
;


decl:
  TWITH lvalue TSEMI
    { $$ = new ExprStmt(new WithExpr($2)); }
| TUSE lvalue TSEMI
    { $$ = new ExprStmt(new UseExpr($2)); }
| TWHERE whereexpr TSEMI
    { $$ = new ExprStmt($2); }
| type_decl
| fn_decl
| mod_decl
;


decl_ls:
  /* empty */
    { $$ = new AList<Stmt>(); }
| decl_ls pragma_ls decl
    {
      $3->copyPragmas(*$2);
      $1->insertAtTail($3);
    }
| decl_ls pragma_ls var_decl
    {
      $3->copyPragmas(*$2);
      $1->add($3);
    }
;


tuple_multiplier:
  intliteral
/* | non_tuple_lvalue */
;


expr_type:
  identifier
    {
      Vec<char*>* new_names = new Vec<char*>();
      new_names->add($1);
      $$ = new UnresolvedType(new_names);
    }
| TQUESTION identifier
    {
      INT_FATAL("Not yet handling ?type construct");
    }
| non_tuple_lvalue TOF identifier
    {
      $$ = new ExprType(
             new ParenOpExpr($1, 
               new AList<Expr>(
                 new Variable(
                   new UnresolvedSymbol($3)))));
    }
| memberaccess_expr
    {
      $$ = new ExprType($1);
    }
| parenop_expr
    {
      $$ = new ExprType($1);
    }
| tuple_multiplier TSTAR identifier
    {
      AList<Expr>* argList = new AList<Expr>(new Variable(new UnresolvedSymbol($3)));
      argList->insertAtTail($1);
      $$ = new ExprType(new ParenOpExpr(new Variable(new UnresolvedSymbol("_htuple")), argList));
    }
;


type:
  domain_type
| index_type
| array_type
| record_tuple_type
| expr_type
;

domain_type:
  TDOMAIN
    { $$ = new DomainType(); }
| TDOMAIN TLP expr TRP
    { $$ = new DomainType($3); }
;


index_type:
  TINDEX
    { $$ = new IndexType(); }
| TINDEX TLP expr TRP
    { $$ = new IndexType($3); }
;


forallExpr:
  TLSBR nonempty_expr_ls TRSBR
    { $$ = Symboltable::startForallExpr($2); }
| TLSBR nonempty_expr_ls TIN nonempty_expr_ls TRSBR
    { $$ = Symboltable::startForallExpr($4, $2); }
;


array_type:
  TLSBR TRSBR type
    { $$ = new ArrayType(unknownDomain, $3); }
| TLSBR TQUESTION identifier TRSBR type
    { 
      Symboltable::defineQueryDomain($3);  // really need to tuck this into
                                           // a var def stmt to be inserted
                                           // as soon as the next stmt is
                                           // defined  -- BLC
      $$ = new ArrayType(unknownDomain, $5);
    }
| forallExpr type
    {
      Symboltable::finishForallExpr($1);
      $$ = new ArrayType($1, $2);
    }
;


statements:
  /* empty */
    { $$ = new AList<Stmt>(); }
| statements pragma_ls statement
    { 
      $3->copyPragmas(*$2);
      $1->insertAtTail($3);
    }
| statements pragma_ls var_decl
    { 
      $3->copyPragmas(*$2);
      $1->add($3);
    }
;


function_body_single_stmt:
  noop_stmt
| if_stmt
| select_stmt
| loop
| call_stmt
| return_stmt
;


function_body_stmt:
  function_body_single_stmt
    { $$ = new BlockStmt(new AList<Stmt>($1)); }
| block_stmt
;


statement:
  noop_stmt
| TLABEL identifier statement
    { $$ = new LabelStmt(new LabelSymbol($2), 
                         new BlockStmt(new AList<Stmt>($3))); }
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
| assign_stmt
| if_stmt
| select_stmt
| loop
| call_stmt
| lvalue TSEMI
    { $$ = new ExprStmt($1); }
| return_stmt
| block_stmt
    { $$ = $1; }
| error
    { printf("syntax error"); exit(1); }
;


pragma_ls:
    { $$ = new Vec<char*>(); }
| pragma_ls pragma
    {
      $1->add($2);
    }
;


pragma:
  TPRAGMA STRINGLITERAL
{ $$ = copystring($2); }
;


call_stmt:
  TCALL lvalue TSEMI
    { $$ = new ExprStmt($2); }
;


noop_stmt:
  TSEMI
    { $$ = new NoOpStmt(); }
;


atomic_cobegin:
    { $$ = BLOCK_NORMAL; }
| TATOMIC
    { $$ = BLOCK_ATOMIC; }
| TCOBEGIN
    { $$ = BLOCK_COBEGIN; }
;


block_stmt:
  atomic_cobegin TLCBR
    {
      $<pblockstmt>$ = Symboltable::startCompoundStmt();
    }
                       statements TRCBR
    {
      $$ = Symboltable::finishCompoundStmt($<pblockstmt>3, $4);
      $$->blockType = $1;
    }
;


return_stmt:
  TRETURN TSEMI
    { $$ = new ReturnStmt(NULL); }
| TRETURN expr TSEMI
    { $$ = new ReturnStmt($2); }
| TYIELD TSEMI
    { $$ = new ReturnStmt(NULL, true); }
| TYIELD expr TSEMI
    { $$ = new ReturnStmt($2, true); }
;


fortype:
  TFOR
    { $$ = false; }
| TFORALL
    { $$ = true; }
;


indexes:
  ident_symbol
    { $$ = new AList<Symbol>($1); }
| indexes TCOMMA ident_symbol
    { $1->insertAtTail($3); }
;


indexlist:
  indexes
| TLP indexes TRP
  { $$ = $2; }
;


forloop:
  fortype indexlist TIN expr
    { 
      $<pforloopstmt>$ = Symboltable::startForLoop($1, $2, $4);
    }
                             block_stmt
    { 
      $$ = Symboltable::finishForLoop($<pforloopstmt>5, $6);
    }
| fortype indexlist TIN expr
    { 
      $<pforloopstmt>$ = Symboltable::startForLoop($1, $2, $4);
    }
                             TDO statement
    { 
      $$ = Symboltable::finishForLoop($<pforloopstmt>5, $7);
    }
| TLSBR indexlist TIN expr TRSBR
    { 
      $<pforloopstmt>$ = Symboltable::startForLoop(true, $2, $4);
    }
                                 statement
    { 
      $$ = Symboltable::finishForLoop($<pforloopstmt>6, $7);
    }
;


whileloop:
TWHILE expr TDO 
    {
      $<pblockstmt>$ = Symboltable::startCompoundStmt();
    }
                statement
    {
      $$ = new WhileLoopStmt(true, $2, Symboltable::finishCompoundStmt($<pblockstmt>4, new AList<Stmt>($5)));
    }
| TDO
    {
      $<pblockstmt>$ = Symboltable::startCompoundStmt();
    }
      statement TWHILE expr TSEMI
    {
      $$ = new WhileLoopStmt(false, $5, Symboltable::finishCompoundStmt($<pblockstmt>2, new AList<Stmt>($3)));
    }
| TWHILE expr block_stmt
    { $$ = new WhileLoopStmt(true, $2, $3); }
;


loop:
  forloop
| whileloop
;


if_stmt:
  TIF expr block_stmt %prec TNOELSE
    { $$ = new CondStmt($2, dynamic_cast<BlockStmt*>($3)); }
| TIF expr TTHEN statement %prec TNOELSE
    { $$ = new CondStmt($2, new BlockStmt(new AList<Stmt>($4))); }
| TIF expr block_stmt TELSE statement
    { $$ = new CondStmt($2, dynamic_cast<BlockStmt*>($3), new BlockStmt(new AList<Stmt>($5))); }
| TIF expr TTHEN statement TELSE statement
    { $$ = new CondStmt($2, new BlockStmt(new AList<Stmt>($4)), new BlockStmt(new AList<Stmt>($6))); }
;


when_stmt:
  TWHEN nonempty_expr_ls TDO statement
    {
      $$ = new WhenStmt($2, new BlockStmt(new AList<Stmt>($4)));
    }
| TWHEN nonempty_expr_ls block_stmt
    {
      $$ = new WhenStmt($2, $3);
    }
| TOTHERWISE statement
    {
      $$ = new WhenStmt(new AList<Expr>(), new BlockStmt(new AList<Stmt>($2)));
    }
;


when_stmts:
  /* empty */
    {
      $$ = new AList<WhenStmt>();
    }
| when_stmts when_stmt
    {
      $1->insertAtTail($2);
    }
; 


select_stmt:
  TSELECT expr TLCBR when_stmts TRCBR
    {
      $$ = new SelectStmt($2, $4);
    }
;


assign_op_tag:
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
| TASSIGNSEQCAT
    { $$ = GETS_SEQCAT; }
;


assign_expr:
  lvalue assign_op_tag expr
    { $$ = new AssignOp($2, $1, $3); }
;


assign_stmt:
  assign_expr TSEMI
    { $$ = new ExprStmt($1); }
;


expr_ls:
  /* empty */
    { $$ = new AList<Expr>(); }
| nonempty_expr_ls
;


expr_list_item:
  identifier TASSIGN expr
    { $$ = new NamedExpr($1, $3); }
| expr
    { $$ = $1; }
;


nonempty_expr_ls:
  pragma_ls expr_list_item
    { $2->copyPragmas(*$1); $$ = new AList<Expr>($2); }
| nonempty_expr_ls TCOMMA pragma_ls expr_list_item
    { $4->copyPragmas(*$3); $1->insertAtTail($4); }
;


tuple_paren_expr:
  TLP nonempty_expr_ls TRP 
    { 
      if ($2->length() == 1) {
        $$ = $2->popHead();
      } else {
        $$ = new Tuple($2);
      }
    }
;


parenop_expr:
  non_tuple_lvalue TLP expr_ls TRP
    { $$ = new ParenOpExpr($1, $3); }
;


memberaccess_expr:
  non_tuple_lvalue TDOT identifier
    { $$ = new MemberAccess($1, new UnresolvedSymbol($3)); }
;


non_tuple_lvalue:
  identifier
    { $$ = new Variable(new UnresolvedSymbol($1)); }
| parenop_expr
| memberaccess_expr
;


lvalue:
  non_tuple_lvalue
| tuple_paren_expr
;


atom:
  literal
| lvalue
;


seq_expr:
  TSEQBEGIN expr_ls TSEQEND
    { $$ = new SeqExpr($2); }
;


if_expr:
  TLP TIF expr TTHEN expr TELSE expr TRP
    {
      $$ = new CondExpr($3, $5, $7);
    }
| TLP TIF expr TTHEN expr TRP
    { $$ = new CondExpr($3, $5); }
;


expr: 
  atom
| TNIL
    { $$ = new Variable(Symboltable::lookupInternal("nil", SCOPE_INTRINSIC)); }
| TLET
    { $<pexpr>$ = Symboltable::startLetExpr(); }
       var_decl_inner_ls TIN expr
    { $$ = Symboltable::finishLetExpr($<pexpr>2, $3, $5); }
| reduction %prec TREDUCE
| expr TCOLON type
  { $$ = new CastExpr($3, $1); }
| expr TCOLON STRINGLITERAL
  { 
    Variable* _tostring =
      new Variable(new UnresolvedSymbol("_tostring"));
    AList<Expr>* args = new AList<Expr>($1);
    args->insertAtTail(new StringLiteral($3));
    $$ = new ParenOpExpr(_tostring, args);
  }
| range %prec TDOTDOT
| if_expr
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
    { $$ = new BinOp(BINOP_BY, $1, $3); }
;

reduction:
  identifier TREDUCE expr
    { $$ = new ReduceExpr(new UnresolvedSymbol($1), $3, false); }
| identifier TSCAN expr
    { $$ = new ReduceExpr(new UnresolvedSymbol($1), $3, true); }
;


range:
  expr TDOTDOT expr
    {
      $$ = new SimpleSeqExpr($1, $3);
    }
;


intliteral:
  INTLITERAL
    { $$ = new IntLiteral(yytext, atol(yytext)); }
;


literal:
  intliteral
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


opt_identifier:
    { $$ = NULL; }
| identifier
;


%%
