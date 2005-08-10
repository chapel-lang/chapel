%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexyacc.h" // all #includes here, for make depend

  static int anon_record_uid = 1;

%}

%start program

%union  {
  bool boolval;
  char* pch;
  Vec<char*>* vpch;

  OpTag ot;
  varType vt;
  consType ct;
  paramType pt;
  blockStmtType blktype;
  fnType ft;
  ForLoopStmtTag flstag;

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
%token TORDERED
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

%token TUNSPECIFIED

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
%token TCOLON
%token TNOTCOLON
%token TQUESTION
%token TLP
%token TRP
%token TSEQBEGIN
%token TSEQEND
%token TLSBR
%token TRSBR
%token TLCBR
%token TRCBR

%type <vt> config_static
%type <ct> var_const_param
 
%type <ot> assign_op_tag
%type <pt> formal_tag
%type <ft> fn_tag
%type <blktype> atomic_cobegin
%type <flstag> for_loop_stmt_tag

%type <boolval> fnretref

%type <pch> identifier fname opt_identifier
%type <pch> pragma
%type <vpch> pragma_ls

%type <pexpr> parenop_expr memberaccess_expr non_tuple_lvalue lvalue
%type <pexpr> tuple_paren_expr atom expr expr_list_item
%type <pexpr> literal range seq_expr where whereexpr
%type <pexpr> tuple_multiplier intliteral variable_expr
%type <pexpr> reduction opt_init_expr assign_expr if_expr
%type <pexprls> expr_ls nonempty_expr_ls tuple_inner_type_ls
%type <pdefexpr> formal enum_item
%type <pdefexprls> formal_ls opt_formal_ls enum_ls
%type <pforallexpr> forallExpr

%type <pstmt> select_stmt label_stmt goto_stmt break_stmt continue_stmt
%type <pstmt> usertype_decl type_decl fn_decl struct_decl mod_decl
%type <pstmt> function_body_single_stmt empty_stmt
%type <pstmt> assign_stmt if_stmt return_stmt loop forloop whileloop enum_decl
%type <pstmt> stmt call_stmt decl typevar_decl
%type <pstmtls> decl_ls stmt_ls modulebody program
%type <pstmtls> var_decl var_decl_inner var_decl_inner_ls record_inner_var_ls
%type <pblockstmt> function_body_stmt block_stmt
%type <pwhenstmt> when_stmt
%type <pwhenstmtls> when_stmt_ls

%type <pexpr> opt_var_type var_type fnrettype
%type <pexpr> type record_tuple_type
%type <pexpr> record_tuple_inner_type
%type <ptype> class_record_union

%type <pfnsym> function

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
  stmt_ls
;


stmt_ls:
  /* empty */
    { $$ = new AList<Stmt>(); }
| stmt_ls pragma_ls stmt
    { 
      $3->copyPragmas(*$2);
      $1->insertAtTail($3);
    }
| stmt_ls pragma_ls var_decl
    { 
      $3->copyPragmas(*$2);
      $1->add($3);
    }
;


function_body_single_stmt:
  empty_stmt
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


stmt:
  empty_stmt
| label_stmt
| goto_stmt
| break_stmt
| continue_stmt
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


label_stmt:
  TLABEL identifier stmt
    { $$ = new LabelStmt(new LabelSymbol($2), 
                         new BlockStmt(new AList<Stmt>($3))); }
;


goto_stmt:
  TGOTO identifier TSEMI
    { $$ = new GotoStmt(goto_normal, $2); }
;


break_stmt:
  TBREAK identifier TSEMI
    { $$ = new GotoStmt(goto_break, $2); }
| TBREAK TSEMI
    { $$ = new GotoStmt(goto_break); }
;


continue_stmt:
  TCONTINUE identifier TSEMI
    { $$ = new GotoStmt(goto_continue, $2); }
| TCONTINUE TSEMI
    { $$ = new GotoStmt(goto_continue); }
;


call_stmt:
  TCALL lvalue TSEMI
    { $$ = new ExprStmt($2); }
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
                       stmt_ls TRCBR
    {
      $$ = Symboltable::finishCompoundStmt($<pblockstmt>3, $4);
      $$->blockType = $1;
    }
;


empty_stmt:
  TSEMI
    {
      $$ = Symboltable::finishCompoundStmt(Symboltable::startCompoundStmt(), NULL);
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


for_loop_stmt_tag:
  TFOR
    { $$ = FORLOOPSTMT_FOR; }
| TORDERED TFORALL
    { $$ = FORLOOPSTMT_ORDEREDFORALL; }
| TFORALL
    { $$ = FORLOOPSTMT_FORALL; }
;


forloop:
  for_loop_stmt_tag nonempty_expr_ls TIN nonempty_expr_ls
    { 
      $<pforloopstmt>$ = Symboltable::startForLoop($1, $2, $4);
    }
                                       block_stmt
    { 
      $$ = Symboltable::finishForLoop($<pforloopstmt>5, $6);
    }
| for_loop_stmt_tag nonempty_expr_ls TIN nonempty_expr_ls
    { 
      $<pforloopstmt>$ = Symboltable::startForLoop($1, $2, $4);
    }
                                       TDO stmt
    { 
      $$ = Symboltable::finishForLoop($<pforloopstmt>5, $7);
    }
| TLSBR nonempty_expr_ls TIN nonempty_expr_ls TRSBR
    { 
      $<pforloopstmt>$ = Symboltable::startForLoop(FORLOOPSTMT_FORALL, $2, $4);
    }
                                 stmt
    { 
      $$ = Symboltable::finishForLoop($<pforloopstmt>6, $7);
    }
;


whileloop:
TWHILE expr TDO 
    {
      $<pblockstmt>$ = Symboltable::startCompoundStmt();
    }
                stmt
    {
      $$ = new WhileLoopStmt(true, $2, Symboltable::finishCompoundStmt($<pblockstmt>4, new AList<Stmt>($5)));
    }
| TDO
    {
      $<pblockstmt>$ = Symboltable::startCompoundStmt();
    }
      stmt TWHILE expr TSEMI
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
| TIF expr TTHEN stmt %prec TNOELSE
    { $$ = new CondStmt($2, new BlockStmt(new AList<Stmt>($4))); }
| TIF expr block_stmt TELSE stmt
    { $$ = new CondStmt($2, dynamic_cast<BlockStmt*>($3), new BlockStmt(new AList<Stmt>($5))); }
| TIF expr TTHEN stmt TELSE stmt
    { $$ = new CondStmt($2, new BlockStmt(new AList<Stmt>($4)), new BlockStmt(new AList<Stmt>($6))); }
;


when_stmt:
  TWHEN nonempty_expr_ls TDO stmt
    {
      $$ = new WhenStmt($2, new BlockStmt(new AList<Stmt>($4)));
    }
| TWHEN nonempty_expr_ls block_stmt
    {
      $$ = new WhenStmt($2, $3);
    }
| TOTHERWISE stmt
    {
      $$ = new WhenStmt(new AList<Expr>(), new BlockStmt(new AList<Stmt>($2)));
    }
;


when_stmt_ls:
  /* empty */
    {
      $$ = new AList<WhenStmt>();
    }
| when_stmt_ls when_stmt
    {
      $1->insertAtTail($2);
    }
; 


select_stmt:
  TSELECT expr TLCBR when_stmt_ls TRCBR
    {
      $$ = new SelectStmt($2, $4);
    }
;


assign_stmt:
  assign_expr TSEMI
    { $$ = new ExprStmt($1); }
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
      Symboltable::continueFnDef($<pfnsym>3, $4, dtUnknown, $5, $7);
    }
                                                         function_body_stmt
    {
      DefExpr* def = new DefExpr(Symboltable::finishFnDef($<pfnsym>3, $9), NULL, $6);
      $$ = new ExprStmt(def);
    }
;


enum_item:
  identifier
    {
      $$ = new DefExpr(new EnumSymbol($1));
    }
| identifier TASSIGN expr
    {
      $$ = new DefExpr(new EnumSymbol($1), $3);
    }
;


enum_ls:
  enum_item
    {
      $$ = new AList<DefExpr>($1);
    }
| enum_ls TCOMMA enum_item
    {
      $1->insertAtTail($3);
      $$ = $1;
    }
;


enum_decl:
  TENUM pragma_ls identifier TLCBR enum_ls TRCBR TSEMI
    {
      EnumType* pdt = new EnumType($5);
      TypeSymbol* pst = new TypeSymbol($3, pdt);
      pst->copyPragmas(*$2);
      pdt->addSymbol(pst);
      DefExpr* def_expr = new DefExpr(pst);
      $$ = new ExprStmt(def_expr);
    }
;


class_record_union:
  TCLASS
    { $$ = new ClassType(CLASS_CLASS); }
| TRECORD
    { $$ = new ClassType(CLASS_RECORD); }
| TUNION
    { $$ = new ClassType(CLASS_UNION); }
;


struct_decl:
  class_record_union pragma_ls identifier TLCBR
    {
      Symboltable::pushScope(SCOPE_CLASS);
    }
                                                decl_ls TRCBR
    {
      SymScope *scope = Symboltable::popScope();
      DefExpr* def = Symboltable::defineStructType($3, $1, scope, $6);
      def->sym->copyPragmas(*$2);
      $$ = new ExprStmt(def);
    }
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
| enum_decl
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


decl:
  TWITH lvalue TSEMI
    { $$ = new ExprStmt(new ImportExpr(IMPORT_WITH, $2)); }
| TUSE lvalue TSEMI
    { $$ = new ExprStmt(new ImportExpr(IMPORT_USE, $2)); }
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
        

record_tuple_inner_type:
  record_inner_var_ls TRP
    {
      SymScope *scope = Symboltable::popScope();
      $$ = Symboltable::defineStructType(glomstrings(2, "_anon_record", intstring(anon_record_uid++)), new ClassType(CLASS_RECORD), scope, $1);
    }
| tuple_inner_type_ls TRP
    {
      Symboltable::popScope();
      char *tupleName = glomstrings(2, "_tuple", intstring($1->length()));
      $$ = new CallExpr(tupleName, $1);
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
      $$ = Symboltable::defineStructType(glomstrings(2, "_anon_record", intstring(anon_record_uid++)), new ClassType(CLASS_RECORD), scope, $4);
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


var_type:
  TCOLON type
    {
      $$ = $2;
    }
| TLIKE expr
    {
      $$ = $2;
    }
;


fnrettype:
  /* empty */
    { $$ = NULL; }
| TCOLON type
    { $$ = $2; }
;


opt_var_type:
  /* nothing */
    {
      $$ = NULL;
    }
| var_type
;


opt_init_expr:
  /* nothing */
    { $$ = NULL; }
| TASSIGN expr
    { $$ = $2; }
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


formal:
  formal_tag pragma_ls identifier opt_var_type opt_init_expr
    {
      $$ = Symboltable::defineParam($1, $3, $4, $5);
      $$->sym->copyPragmas(*$2);
    }
| TTYPE pragma_ls identifier opt_var_type opt_init_expr
    {
      DefExpr* defExpr = Symboltable::defineParam(PARAM_TYPE, $3, $4, $5);
      defExpr->sym->copyPragmas(*$2);
      ParamSymbol* ps = dynamic_cast<ParamSymbol*>(defExpr->sym);
      char *name = glomstrings(2, "__type_variable_", ps->name);
      VariableType* new_type = new VariableType(getMetaType(NULL));
      TypeSymbol* new_type_symbol = new TypeSymbol(name, new_type);
      new_type->addSymbol(new_type_symbol);
      ps->type = getMetaType(NULL);
      ps->typeVariable = new_type_symbol;
      $$ = defExpr;
    }
| TLP formal_ls TRP
    {
      ClassType *t = new ClassType(CLASS_RECORD);
      // NOTE:
      //   change DefExpr into ExprStmt because functions take 
      //   a list of DefExpr but records take a list of Stmt
      // NOTE:
      //   this record has members which are ParamSymbols
      AList<Stmt> *stmts = new AList<Stmt>;
      for_alist(DefExpr, x, $2) {
        stmts->insertAtTail(new ExprStmt(x));
      }
      Symboltable::defineStructType(NULL, t, Symboltable::getCurrentScope(), stmts);
      $$ = Symboltable::defineParam(PARAM_IN, "<anonymous>", NULL, NULL);
      $$->sym->type = t;
      t->isPattern = true;
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
| TNOT
  { $$ = "not"; }
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
    { $$ = new CallExpr(OP_LOGNOT, $2); }
| TBNOT whereexpr
    { $$ = new CallExpr(OP_BITNOT, $2); }
| whereexpr TPLUS whereexpr
    { $$ = new CallExpr(OP_PLUS, $1, $3); }
| whereexpr TMINUS whereexpr
    { $$ = new CallExpr(OP_MINUS, $1, $3); }
| whereexpr TSTAR whereexpr
    { $$ = new CallExpr(OP_MULT, $1, $3); }
| whereexpr TDIVIDE whereexpr
    { $$ = new CallExpr(OP_DIV, $1, $3); }
| whereexpr TMOD whereexpr
    { $$ = new CallExpr(OP_MOD, $1, $3); }
| whereexpr TEQUAL whereexpr
    { $$ = new CallExpr(OP_EQUAL, $1, $3); }
| whereexpr TNOTEQUAL whereexpr
    { $$ = new CallExpr(OP_NEQUAL, $1, $3); }
| whereexpr TLESSEQUAL whereexpr
    { $$ = new CallExpr(OP_LEQUAL, $1, $3); }
| whereexpr TGREATEREQUAL whereexpr
    { $$ = new CallExpr(OP_GEQUAL, $1, $3); }
| whereexpr TLESS whereexpr
    { $$ = new CallExpr(OP_LTHAN, $1, $3); }
| whereexpr TGREATER whereexpr
    { $$ = new CallExpr(OP_GTHAN, $1, $3); }
| whereexpr TBAND whereexpr
    { $$ = new CallExpr(OP_BITAND, $1, $3); }
| whereexpr TBOR whereexpr
    { $$ = new CallExpr(OP_BITOR, $1, $3); }
| whereexpr TBXOR whereexpr
    { $$ = new CallExpr(OP_BITXOR, $1, $3); }
| whereexpr TCOMMA whereexpr
    { $$ = new CallExpr(OP_LOGAND, $1, $3); }
| whereexpr TOR whereexpr
    { $$ = new CallExpr(OP_LOGOR, $1, $3); }
| whereexpr TEXP whereexpr
    { $$ = new CallExpr(OP_EXP, $1, $3); }
| whereexpr TSEQCAT whereexpr
    { $$ = new CallExpr(OP_SEQCAT, $1, $3); }
| whereexpr TBY whereexpr
    { $$ = new CallExpr(OP_BY, $1, $3); }
| whereexpr TCOLON whereexpr
    { $$ = new CallExpr(OP_SUBTYPE, $1, $3); }
| whereexpr TNOTCOLON whereexpr
    { $$ = new CallExpr(OP_NOTSUBTYPE, $1, $3); }
| class_record_union pragma_ls opt_identifier TLCBR decl_ls TRCBR
    { $$ = NULL; }
;


function:
  fname
    { $$ = new FnSymbol($1); }
| TASSIGN identifier
    { $$ = new FnSymbol($2, NULL, true); }
| identifier TDOT fname
    { $$ = new FnSymbol($3, new TypeSymbol($1, NULL)); }
| identifier TDOT TASSIGN identifier
    { $$ = new FnSymbol($4, new TypeSymbol($1, NULL), true); }
;


tuple_multiplier:
  intliteral
/* | non_tuple_lvalue */
;


variable_expr:
  identifier
    {
      $$ = new Variable(new UnresolvedSymbol($1));
    }
;


type:
  variable_expr
| memberaccess_expr
| parenop_expr
| record_tuple_type
| TQUESTION identifier
    {
      INT_FATAL("Not yet handling ?type construct");
    }
| non_tuple_lvalue TOF variable_expr
    {
      $$ = new CallExpr($1, new NamedExpr("elt_type", $3));
    }
| tuple_multiplier TSTAR variable_expr
    {
      $$ = new CallExpr("_htuple", $3, $1);
    }
;


forallExpr:
  TLSBR nonempty_expr_ls TIN nonempty_expr_ls TRSBR
    { $$ = Symboltable::startForallExpr($2, $4); }
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


assign_op_tag:
  TASSIGN
    { $$ = OP_GETSNORM; }
| TASSIGNPLUS
    { $$ = OP_GETSPLUS; }
| TASSIGNMINUS
    { $$ = OP_GETSMINUS; }
| TASSIGNMULTIPLY
    { $$ = OP_GETSMULT; }
| TASSIGNDIVIDE
    { $$ = OP_GETSDIV; }
| TASSIGNBAND
    { $$ = OP_GETSBITAND; }
| TASSIGNBOR
    { $$ = OP_GETSBITOR; }
| TASSIGNBXOR
    { $$ = OP_GETSBITXOR; }
| TASSIGNSEQCAT
    { $$ = OP_GETSSEQCAT; }
;


assign_expr:
  lvalue assign_op_tag expr
    { $$ = new CallExpr($2, $1, $3); }
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
        AList<Expr>* argList = new AList<Expr>();
        int size = 0;
        for_alist(Expr, expr, $2) {
          argList->insertAtTail(new Variable(dtUnknown->symbol));
        }
        for_alist(Expr, expr, $2) {
          argList->insertAtTail(
            new NamedExpr(
              glomstrings(2, "_f", intstring(++size)),
              expr->copy()));
        }
        $$ = new CallExpr(glomstrings(2, "_tuple", intstring(size)), argList);
      }
    }
;


parenop_expr:
  non_tuple_lvalue TLP expr_ls TRP
    { $$ = new CallExpr($1, $3); }
;


memberaccess_expr:
  non_tuple_lvalue TDOT identifier
    { $$ = new MemberAccess($1, new UnresolvedSymbol($3)); }
;


non_tuple_lvalue:
  variable_expr
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
    {
      Expr* seqLiteral = new CallExpr("seq", new Variable(dtUnknown->symbol));
      for_alist(Expr, element, $2) {
        element->remove();
        seqLiteral =
          new CallExpr(
            new MemberAccess(
              seqLiteral,
              new UnresolvedSymbol("_append_in_place")),
            new AList<Expr>(element));
      }
      $$ = seqLiteral;
    }
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
| TUNSPECIFIED
    { $$ = new Variable(Symboltable::lookupInternal("_", SCOPE_INTRINSIC)); }
| TLET
    { $<pexpr>$ = Symboltable::startLetExpr(); }
       var_decl_inner_ls TIN expr
    { $$ = Symboltable::finishLetExpr($<pexpr>2, $3, $5); }
| reduction %prec TREDUCE
| expr TCOLON type
    {
      $$ = new CastExpr($1, $3);
    }
| expr TCOLON STRINGLITERAL
  { 
    $$ = new CallExpr("_tostring", $1, new StringLiteral($3));
  }
| range %prec TDOTDOT
| if_expr
| seq_expr
| forallExpr expr %prec TRSBR
    { $$ = Symboltable::finishForallExpr($1, $2); }
| TPLUS expr %prec TUPLUS
    { $$ = new CallExpr(OP_UNPLUS, $2); }
| TMINUS expr %prec TUMINUS
    { $$ = new CallExpr(OP_UNMINUS, $2); }
| TNOT expr
    { $$ = new CallExpr(OP_LOGNOT, $2); }
| TBNOT expr
    { $$ = new CallExpr(OP_BITNOT, $2); }
| expr TPLUS expr
    { $$ = new CallExpr(OP_PLUS, $1, $3); }
| expr TMINUS expr
    { $$ = new CallExpr(OP_MINUS, $1, $3); }
| expr TSTAR expr
    { $$ = new CallExpr(OP_MULT, $1, $3); }
| expr TDIVIDE expr
    { $$ = new CallExpr(OP_DIV, $1, $3); }
| expr TMOD expr
    { $$ = new CallExpr(OP_MOD, $1, $3); }
| expr TEQUAL expr
    { $$ = new CallExpr(OP_EQUAL, $1, $3); }
| expr TNOTEQUAL expr
    { $$ = new CallExpr(OP_NEQUAL, $1, $3); }
| expr TLESSEQUAL expr
    { $$ = new CallExpr(OP_LEQUAL, $1, $3); }
| expr TGREATEREQUAL expr
    { $$ = new CallExpr(OP_GEQUAL, $1, $3); }
| expr TLESS expr
    { $$ = new CallExpr(OP_LTHAN, $1, $3); }
| expr TGREATER expr
    { $$ = new CallExpr(OP_GTHAN, $1, $3); }
| expr TBAND expr
    { $$ = new CallExpr(OP_BITAND, $1, $3); }
| expr TBOR expr
    { $$ = new CallExpr(OP_BITOR, $1, $3); }
| expr TBXOR expr
    { $$ = new CallExpr(OP_BITXOR, $1, $3); }
| expr TAND expr
    { $$ = new CallExpr(OP_LOGAND, $1, $3); }
| expr TOR expr
    { $$ = new CallExpr(OP_LOGOR, $1, $3); }
| expr TEXP expr
    { $$ = new CallExpr(OP_EXP, $1, $3); }
| expr TSEQCAT expr
    { $$ = new CallExpr(OP_SEQCAT, $1, $3); }
| expr TBY expr
    { $$ = new CallExpr(OP_BY, $1, $3); }
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
      $$ = new CallExpr("_aseq", $1, $3, new IntLiteral(1));
    }
;


intliteral:
  INTLITERAL
    { $$ = new IntLiteral(yytext); }
;


literal:
  intliteral
| FLOATLITERAL
    { $$ = new FloatLiteral(yytext, atof(yytext)); }
| COMPLEXLITERAL
    {
      yytext[strlen(yytext)-1] = '\0';
      $$ = new CallExpr("complex", new FloatLiteral(yytext, atof(yytext)));
    }
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
