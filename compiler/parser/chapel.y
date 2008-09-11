/*** Chapel Parser

***********
CONVENTIONS
***********

opt -- optional
ls -- list, as in a list of expressions or statements
expr -- expression
stmt -- statement
type -- type, parsed as an expression

*****
NOTES
*****

***/


%{

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "lexyacc.h" // all #includes here, for make depend

  static int anon_record_uid = 1;
  static int query_uid = 1;
  int captureTokens;
  char captureString[1024];

#define YYLLOC_DEFAULT(Current, Rhs, N)          \
  if (N) { \
    (Current).first_line   = (Rhs)[1].first_line;      \
    if ((Current).first_line) yystartlineno = (Current).first_line; \
    (Current).first_column = (Rhs)[1].first_column;    \
    (Current).last_line    = (Rhs)[N].last_line;       \
    (Current).last_column  = (Rhs)[N].last_column; \
  } else (Current) = yylloc;

%}

%start program

%union  {
  const char* pch;
  Vec<const char*>* vpch;

  RetTag retTag;

  bool b;
  IntentTag pt;

  Expr* pexpr;
  DefExpr* pdefexpr;
  CallExpr* pcallexpr;
  BlockStmt* pblockstmt;


  Type* ptype;
  EnumType* pet;

  Symbol* psym;
  FnSymbol* pfnsym;
  ModuleSymbol* pmodsym;
  EnumSymbol* penumsym;
}

%token TATOMIC
%token TBEGIN
%token TBREAK
%token TCLASS
%token TCOBEGIN
%token TCOFORALL
%token TCOMPILERERROR
%token TCOMPILERWARNING
%token TCONFIG
%token TCONST
%token TCONTINUE
%token TDEF
%token TDELETE
%token TDISTRIBUTED
%token TDO
%token TDOMAIN
%token TENUM
%token TEXTERN
%token TFOR
%token TFORALL
%token TGOTO
%token TIF
%token TIN
%token TINDEX
%token TINOUT
%token TITERATOR
%token TLABEL
%token TLET
%token TLOCAL
%token TMINUSMINUS
%token TMODULE
%token TNEW
%token TNIL
%token TON
%token TOTHERWISE
%token TOUT
%token TPARAM
%token TPLUSPLUS
%token TPRAGMA
%token TPRIMITIVE
%token TRECORD
%token TRETURN
%token TSELECT
%token TSINGLE
%token TSERIAL
%token TSPARSE
%token TSUBDOMAIN
%token TSYNC
%token TTHEN
%token TTYPE
%token TUNION
%token TUSE
%token TVAL
%token TVAR
%token TWHEN
%token TWHERE
%token TWHILE
%token TYIELD

%token TIDENT
%token INTLITERAL REALLITERAL IMAGLITERAL
%token <pch> STRINGLITERAL

%token TALIAS

%token TASSIGN
%token TSWAP
%token TASSIGNBAND
%token TASSIGNBOR
%token TASSIGNBXOR
%token TASSIGNDIVIDE
%token TASSIGNEXP
%token TASSIGNLAND
%token TASSIGNLOR
%token TASSIGNMINUS
%token TASSIGNMOD
%token TASSIGNMULTIPLY
%token TASSIGNPLUS
%token TASSIGNSR
%token TASSIGNSL

%token TCOLON
%token TCOMMA
%token TDOT
%token TDOTDOTDOT
%token TQUESTION

%token TLCBR TRCBR
%token TLP TRP
%token TLSBR TRSBR

%token TSEMI

%type <b> is_config
 
%type <pt> intent_tag

%type <retTag> ret_tag

%type <pch> identifier fn_identifier opt_identifier
%type <pch> pragma
%type <vpch> pragma_ls

%type <pblockstmt> program stmt_ls
%type <pblockstmt> class_body_stmt_ls

%type <pblockstmt> stmt empty_stmt label_stmt goto_stmt break_stmt continue_stmt
%type <pblockstmt> expr_stmt if_stmt param_for_stmt for_stmt forall_stmt coforall_stmt while_do_stmt do_while_stmt serial_stmt
%type <pblockstmt> select_stmt return_stmt yield_stmt assign_stmt class_body_stmt
%type <pblockstmt> type_select_stmt on_stmt non_empty_stmt use_stmt_ls sync_stmt

%type <pblockstmt> typedef_decl_stmt typedef_decl_stmt_inner fn_decl_stmt class_decl_stmt mod_decl_stmt extern_fn_decl_stmt
%type <pblockstmt> typevar_decl_stmt enum_decl_stmt use_stmt delete_stmt

%type <pblockstmt> var_decl_stmt var_decl_stmt_inner_ls
%type <pblockstmt> var_decl_stmt_inner tuple_var_decl_stmt_inner_ls

%type <pblockstmt> block_stmt function_body_stmt
%type <pblockstmt> begin_stmt cobegin_stmt atomic_stmt local_stmt

%type <pexpr> when_stmt
%type <pblockstmt> when_stmt_ls

%type <pexpr> opt_type opt_formal_type array_type
%type <pexpr> type anon_record_type type_binding_part opt_domain
%type <ptype> class_tag
%type <pet> enum_ls

%type <pexpr> parenop_expr memberaccess_expr non_tuple_lvalue lvalue
%type <pexpr> tuple_paren_expr expr expr_list_item opt_return_part
%type <pexpr> literal opt_where_part distributed_expr
%type <pexpr> variable_expr stmt_level_expr alias_expr formal_level_type
%type <pexpr> reduction opt_init_expr opt_init_type var_arg_expr
%type <pdefexpr> formal enum_item
%type <pdefexpr> query_expr

%type <pexpr> formal_type_expr formal_tuple_paren_expr formal_parenop_expr
%type <pexpr> formal_memberaccess_expr formal_expr_list_item

%type <pcallexpr> opt_inherit_expr_ls expr_ls nonempty_expr_ls
%type <pcallexpr> formal_expr_ls nonempty_formal_expr_ls

%type <pfnsym> fn_decl_stmt_inner formal_ls opt_formal_ls

/* These are declared in increasing order of precedence. */

%left TNOELSE
%left TELSE
%left TCOMMA
%left TFOR
%left TFORALL
%left TIF
%left TBY THASH
%left TRSBR
%left TIN
%left TDOTDOT
%right TSTARTUPLE
%left TOR
%left TAND
%left TBOR
%left TBXOR
%left TBAND
%left TEQUAL TNOTEQUAL
%left TLESSEQUAL TGREATEREQUAL TLESS TGREATER
%left TSHIFTLEFT TSHIFTRIGHT
%left TPLUS TMINUS
%right TUPLUS TUMINUS 
%left TSTAR TDIVIDE TMOD
%right TBNOT TNOT
%left TREDUCE TSCAN
%right TEXP
%left TCOLON
%left TDOT TLP TLSBR

%% 


program: stmt_ls
    { (void)@1.first_line; yyblock = $$; }
;


/** PRAGMAS ******************************************************************/


pragma_ls:
    { $$ = new Vec<const char*>(); }
| pragma_ls pragma
    { $1->add($2); }
;


pragma:
  TPRAGMA STRINGLITERAL
    { $$ = astr($2); }
;


/** STATEMENTS ***************************************************************/


stmt_ls:
  /* nothing */
    { $$ = new BlockStmt(); }
| stmt_ls pragma_ls stmt
    {
      if (DefExpr* def = toDefExpr($3->body.first()))
        def->sym->addFlags($2);
      delete $2;
      $1->insertAtTail($3);
    }
;


stmt:
  empty_stmt
| non_empty_stmt
;


non_empty_stmt:
  label_stmt
| goto_stmt
| break_stmt
| continue_stmt
| expr_stmt
| if_stmt
| param_for_stmt
| for_stmt
| forall_stmt
| coforall_stmt
| while_do_stmt
| do_while_stmt
| select_stmt
| type_select_stmt
| return_stmt
| yield_stmt
| delete_stmt
| assign_stmt
| block_stmt
| use_stmt
| mod_decl_stmt
| fn_decl_stmt
| extern_fn_decl_stmt
| class_decl_stmt
| enum_decl_stmt
| typedef_decl_stmt
| var_decl_stmt
| on_stmt
| sync_stmt
| serial_stmt
| begin_stmt
| cobegin_stmt
| atomic_stmt
| local_stmt
| error
    { printf("syntax error"); clean_exit(1); }
;


class_body_stmt_ls:
  /* nothing */
    { $$ = new BlockStmt(); }
| class_body_stmt_ls pragma_ls class_body_stmt
    {
      if (DefExpr* def = toDefExpr($3->body.first()))
        def->sym->addFlags($2);
      delete $2;
      $1->insertAtTail($3);
    }
;


class_body_stmt:
  fn_decl_stmt
| class_decl_stmt
| enum_decl_stmt
| typevar_decl_stmt
| var_decl_stmt
;


empty_stmt:
  TSEMI
    { $$ = buildChapelStmt(new BlockStmt()); }
;


label_stmt:
  TLABEL identifier stmt
    { $$ = buildLabelStmt($2, $3); }
;


goto_stmt:
  TGOTO identifier TSEMI
    { $$ = buildChapelStmt(new GotoStmt(GOTO_NORMAL, $2)); }
;


break_stmt:
  TBREAK opt_identifier TSEMI
    { $$ = buildChapelStmt(new GotoStmt(GOTO_BREAK, $2)); }
;


continue_stmt:
  TCONTINUE opt_identifier TSEMI
    { $$ = buildChapelStmt(new GotoStmt(GOTO_CONTINUE, $2)); }
;


expr_stmt:
  stmt_level_expr TSEMI
    { $$ = buildChapelStmt($1); }
;


if_stmt:
  TIF expr block_stmt                   %prec TNOELSE
    { $$ = buildIfStmt($2, $3); }
| TIF expr TTHEN stmt                   %prec TNOELSE
    { $$ = buildIfStmt($2, $4); }
| TIF expr block_stmt TELSE stmt
    { $$ = buildIfStmt($2, $3, $5); }
| TIF expr TTHEN stmt TELSE stmt
    { $$ = buildIfStmt($2, $4, $6); }
;


param_for_stmt:
  TFOR TPARAM identifier TIN expr TDO stmt
    { $$ = buildParamForLoopStmt($3, $5, $7); }
| TFOR TPARAM identifier TIN expr block_stmt
    { $$ = buildParamForLoopStmt($3, $5, $6); }
;


for_stmt:
  TFOR expr TIN expr block_stmt
    { $$ = buildForLoopStmt($2, $4, $5); }
| TFOR expr TIN expr TDO stmt
    { $$ = buildForLoopStmt($2, $4, new BlockStmt($6)); }
| TFOR expr block_stmt
    { $$ = buildForLoopStmt(NULL, $2, $3); }
| TFOR expr TDO stmt
    { $$ = buildForLoopStmt(NULL, $2, new BlockStmt($4)); }
;


forall_stmt:
  TFORALL expr TIN expr block_stmt
    { $$ = buildForallLoopStmt($2, $4, $5); }
| TFORALL expr TIN expr TDO stmt
    { $$ = buildForallLoopStmt($2, $4, new BlockStmt($6)); }
| TFORALL expr block_stmt
    { $$ = buildForallLoopStmt(NULL, $2, $3); }
| TFORALL expr TDO stmt
    { $$ = buildForallLoopStmt(NULL, $2, new BlockStmt($4)); }
| TLSBR nonempty_expr_ls TIN expr TRSBR stmt
    {
      if ($2->argList.length() != 1)
        USR_FATAL($4, "invalid index expression");
      $$ = buildForallLoopStmt($2->get(1)->remove(), $4, new BlockStmt($6));
    }
| TLSBR nonempty_expr_ls TRSBR non_empty_stmt
    {
      if ($2->argList.length() != 1)
        USR_FATAL($4, "invalid loop expression");
      $$ = buildForallLoopStmt(NULL, $2->get(1)->remove(), new BlockStmt($4));
    }
;


coforall_stmt:
  TCOFORALL expr TIN expr block_stmt
    { $$ = buildCoforallLoopStmt($2, $4, $5); }
| TCOFORALL expr TIN expr TDO stmt
    { $$ = buildCoforallLoopStmt($2, $4, new BlockStmt($6)); }
| TCOFORALL expr block_stmt
    { $$ = buildCoforallLoopStmt(NULL, $2, $3); }
| TCOFORALL expr TDO stmt
    { $$ = buildCoforallLoopStmt(NULL, $2, new BlockStmt($4)); }
;


while_do_stmt:
  TWHILE expr TDO stmt
    { $$ = buildWhileDoLoopStmt($2, new BlockStmt($4)); }
| TWHILE expr block_stmt
    { $$ = buildWhileDoLoopStmt($2, $3); }
;


do_while_stmt:
TDO stmt TWHILE expr TSEMI
    { $$ = buildDoWhileLoopStmt($4, $2); }
;


type_select_stmt:
  TTYPE TSELECT nonempty_expr_ls TLCBR when_stmt_ls TRCBR
    { $$ = buildTypeSelectStmt($3, $5); }
;


select_stmt:
  TSELECT expr TLCBR when_stmt_ls TRCBR
    { $$ = buildChapelStmt(buildSelectStmt($2, $4)); }
;


when_stmt_ls:
  /* nothing */
    { $$ = buildChapelStmt(); }
| when_stmt_ls when_stmt
    { $1->insertAtTail($2); }
; 


when_stmt:
  TWHEN nonempty_expr_ls TDO stmt
    { $$ = new CondStmt(new CallExpr(PRIMITIVE_WHEN, $2), $4); }
| TWHEN nonempty_expr_ls block_stmt
    { $$ = new CondStmt(new CallExpr(PRIMITIVE_WHEN, $2), $3); }
| TOTHERWISE stmt
    { $$ = new CondStmt(new CallExpr(PRIMITIVE_WHEN), $2); }
;


return_stmt:
  TRETURN opt_return_part TSEMI
    { $$ = buildChapelStmt(new CallExpr(PRIMITIVE_RETURN, $2)); }
;


yield_stmt:
  TYIELD opt_return_part TSEMI
    { $$ = buildChapelStmt(new CallExpr(PRIMITIVE_YIELD, $2)); }
;


delete_stmt:
  TDELETE expr TSEMI
    { $$ = buildChapelStmt(new CallExpr(PRIMITIVE_DELETE, $2)); }
;


opt_return_part:
  /* nothing */
    { $$ = new SymExpr(gVoid); }
| expr
;


assign_stmt:
  lvalue TASSIGN expr TSEMI
    { $$ = buildChapelStmt(new CallExpr("=", $1, $3)); }
| lvalue TASSIGNPLUS expr TSEMI
    { $$ = buildCompoundAssignment("+", $1, $3); }
| lvalue TASSIGNMINUS expr TSEMI
    { $$ = buildCompoundAssignment("-", $1, $3); }
| lvalue TASSIGNMULTIPLY expr TSEMI
    { $$ = buildCompoundAssignment("*", $1, $3); }
| lvalue TASSIGNDIVIDE expr TSEMI
    { $$ = buildCompoundAssignment("/", $1, $3); }
| lvalue TASSIGNMOD expr TSEMI
    { $$ = buildCompoundAssignment("%", $1, $3); }
| lvalue TASSIGNEXP expr TSEMI
    { $$ = buildCompoundAssignment("**", $1, $3); }
| lvalue TASSIGNBAND expr TSEMI
    { $$ = buildCompoundAssignment("&", $1, $3); }
| lvalue TASSIGNBOR expr TSEMI
    { $$ = buildCompoundAssignment("|", $1, $3); }
| lvalue TASSIGNBXOR expr TSEMI
    { $$ = buildCompoundAssignment("^", $1, $3); }
| lvalue TASSIGNLAND expr TSEMI
    { $$ = buildLogicalAndExprAssignment($1, $3); }
| lvalue TASSIGNLOR expr TSEMI
    { $$ = buildLogicalOrExprAssignment($1, $3); }
| lvalue TASSIGNSR expr TSEMI
    { $$ = buildCompoundAssignment(">>", $1, $3); }
| lvalue TASSIGNSL expr TSEMI
    { $$ = buildCompoundAssignment("<<", $1, $3); }
| lvalue TSWAP expr TSEMI
    { $$ = buildChapelStmt(new CallExpr("_chpl_swap", $1, $3)); }
;


block_stmt:
  TLCBR stmt_ls TRCBR
    { $$ = buildChapelStmt($2); }
;


cobegin_stmt:
  TCOBEGIN block_stmt
    {
      $2->lineno = yystartlineno; /* capture line number in case there
                                   * is a need to issue a warning */
      $$ = buildCobeginStmt($2);
    }
;


atomic_stmt:
  TATOMIC stmt
    { $$ = buildAtomicStmt($2); }
;


begin_stmt:
  TBEGIN stmt
    { $$ = buildBeginStmt($2); }
;


on_stmt:
  TON expr TDO stmt
    { $$ = buildOnStmt($2, $4); }
| TON expr block_stmt
    { $$ = buildOnStmt($2, $3); }
;


local_stmt:
  TLOCAL stmt
  {
    $$ = buildLocalStmt($2);
  }
;


sync_stmt:
  TSYNC stmt
    { $$ = buildSyncStmt($2); }
;


serial_stmt:
  TSERIAL expr block_stmt
    { $$ = buildSerialStmt($2, $3); }
| TSERIAL expr TDO stmt
    { $$ = buildSerialStmt($2, $4); }
;



/** DECLARATION STATEMENTS ***************************************************/


use_stmt:
  TUSE use_stmt_ls TSEMI
    { $$ = $2; }
;


use_stmt_ls:
  lvalue
    { $$ = buildChapelStmt(new CallExpr(PRIMITIVE_USE, $1)); }
| use_stmt_ls TCOMMA lvalue
    {
      $1->insertAtTail(new CallExpr(PRIMITIVE_USE, $3));
      $$ = $1;
    }
;


mod_decl_stmt:
  TMODULE identifier TLCBR stmt_ls TRCBR
    { $$ = buildChapelStmt(new DefExpr(buildModule($2, MOD_USER, $4, yyfilename))); }
;


formal_ls:
  /* nothing */
    { $$ = new FnSymbol("_"); }
| formal
    {
      $$ = new FnSymbol("_");
      $$->insertFormalAtTail($1);
    }
| TLP tuple_var_decl_stmt_inner_ls TRP
    {
      $$ = new FnSymbol("_");
      buildTupleArg($$, $2, NULL);
    }
| formal_ls TCOMMA formal
    { $1->insertFormalAtTail($3); }
| formal_ls TCOMMA TLP tuple_var_decl_stmt_inner_ls TRP
    { buildTupleArg($1, $4, NULL); }
;


opt_formal_ls:
  { $$ = new FnSymbol("_"); $$->addFlag(FLAG_NO_PARENS); }
| TLP formal_ls TRP
  { $$ = $2; }
;


fn_decl_stmt_inner:
  fn_identifier opt_formal_ls
    {
      $$ = $2;
      $$->name = astr($1);
      if ($1[0] == '~' && $1[1] != '\0') {
        $$->cname = astr("chpl__destroy_", &($1[1]));
        $$->addFlag(FLAG_DESTRUCTOR);
      } else
        $$->cname = $$->name;
    }
| type_binding_part TDOT fn_identifier opt_formal_ls
    {
      $$ = $4;
      $$->name = astr($3);
      if ($3[0] == '~' && $3[1] != '\0') {
        $$->cname = astr("chpl__destroy_", &($3[1]));
        $$->addFlag(FLAG_DESTRUCTOR);
      } else
        $$->cname = $$->name;
      $$->_this = new ArgSymbol(INTENT_BLANK, "this", dtUnknown, $1);
      $$->insertFormalAtHead(new DefExpr($$->_this));
      $$->insertFormalAtHead(new DefExpr(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken)));
    }
;


ret_tag:
/* none */
    { $$ = RET_VALUE; }
| TCONST
    { $$ = RET_VALUE; }
| TVAR
    { $$ = RET_VAR; }
| TPARAM
    { $$ = RET_PARAM; }
| TTYPE
    { $$ = RET_TYPE; }
;


fn_decl_stmt:
  TDEF
    {
      captureTokens = 1;
      captureString[0] = '\0';
    }
  fn_decl_stmt_inner
    {
      captureTokens = 0;
      $3->userString = astr(captureString);
    }
  ret_tag opt_type opt_where_part function_body_stmt
    {
      $3->retTag = $5;
      if ($5 == RET_VAR)
        $3->setter = new DefExpr(new ArgSymbol(INTENT_BLANK, "setter", dtBool));
      if ($6)
        $3->retExprType = new BlockStmt($6, BLOCK_SCOPELESS);
      if ($7)
        $3->where = new BlockStmt($7);
      $3->insertAtTail($8);
      $$ = buildChapelStmt(new DefExpr($3));
    }
;


function_body_stmt:
  block_stmt
| return_stmt
  { $$ = new BlockStmt($1); }
;


extern_fn_decl_stmt:
  TEXTERN TDEF fn_decl_stmt_inner opt_type TSEMI
    {
      FnSymbol* fn = $3;
      fn->addFlag(FLAG_EXTERN);
      if ($4)
        fn->retExprType = new BlockStmt($4, BLOCK_SCOPELESS);
      else
        fn->retType = dtVoid;
      $$ = buildChapelStmt(new DefExpr(fn));
    }
;


query_expr:
  TQUESTION identifier
    {
      $$ = new DefExpr(new VarSymbol($2));
    }
| TQUESTION
    {
      $$ = new DefExpr(new VarSymbol(astr("_query_dummy", istr(query_uid++))));
    }
;


var_arg_expr:
  TDOTDOTDOT expr
    { $$ = $2; }
| TDOTDOTDOT query_expr
    {
      $2->sym->addFlag(FLAG_PARAM);
      $$ = $2;
    }
;


formal:
  intent_tag identifier opt_formal_type opt_init_expr
    { $$ = buildArgDefExpr($1, $2, $3, $4, NULL); }
| intent_tag identifier opt_formal_type var_arg_expr
    { $$ = buildArgDefExpr($1, $2, $3, NULL, $4); }
;


intent_tag:
  /* nothing */
    { $$ = INTENT_BLANK; }
| TIN
    { $$ = INTENT_IN; }
| TINOUT
    { $$ = INTENT_INOUT; }
| TOUT
    { $$ = INTENT_OUT; }
| TCONST
    { $$ = INTENT_CONST; }
| TPARAM
    { $$ = INTENT_PARAM; }
| TTYPE
    { $$ = INTENT_TYPE; }
;


fn_identifier:
  identifier
| TBNOT identifier
    { $$ = astr("~", $2); }
| TASSIGN 
  { $$ = "="; } 
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
| TSHIFTLEFT
  { $$ = "<<"; }
| TSHIFTRIGHT
  { $$ = ">>"; }
| TMOD
  { $$ = "%"; } 
| TEXP
  { $$ = "**"; } 
| TNOT
  { $$ = "!"; }
| TBY
  { $$ = "by"; } 
| THASH
  { $$ = "#"; }
;


opt_where_part:
  /* nothing */
    { $$ = NULL; }
| TWHERE expr
    { $$ = $2; }
;


type_binding_part:
  variable_expr
  /* | parenop_expr */;


class_decl_stmt:
  class_tag identifier opt_inherit_expr_ls TLCBR class_body_stmt_ls TRCBR
    {
      DefExpr* def = buildClassDefExpr($2, $1, $5);
      ClassType* ct = toClassType(toTypeSymbol(def->sym)->type);
      ct->inherits.insertAtTail($3);
      $$ = buildChapelStmt(def);
    }
;


class_tag:
  TCLASS
    { $$ = new ClassType(CLASS_CLASS); }
| TRECORD
    { $$ = new ClassType(CLASS_RECORD); }
| TUNION
    { $$ = new ClassType(CLASS_UNION); }
;


opt_inherit_expr_ls:
  /* nothing */
    { $$ = new CallExpr(PRIMITIVE_ACTUALS_LIST); }
| TCOLON nonempty_expr_ls
    { $$ = $2; }
;


enum_decl_stmt:
  TENUM identifier TLCBR enum_ls TRCBR TSEMI
    {
      EnumType* pdt = $4;
      TypeSymbol* pst = new TypeSymbol($2, pdt);
      $4->symbol = pst;
      $$ = buildChapelStmt(new DefExpr(pst));
    }
;


enum_ls:
  enum_item
    {
      $$ = new EnumType();
      $1->sym->type = $$;
      $$->constants.insertAtTail($1);
      $$->defaultValue = $1->sym;
    }
| enum_ls TCOMMA enum_item
    {
      $1->constants.insertAtTail($3);
      $3->sym->type = $1;
    }
;


enum_item:
  identifier
    { $$ = new DefExpr(new EnumSymbol($1)); }
| identifier TASSIGN expr
    { $$ = new DefExpr(new EnumSymbol($1), $3); }
;


typedef_decl_stmt_inner:
  identifier TASSIGN type
    {
      VarSymbol* var = new VarSymbol($1);
      var->addFlag(FLAG_TYPE_VARIABLE);
      DefExpr* def = new DefExpr(var, $3);
      $$ = buildChapelStmt(def);
    }
| identifier TASSIGN type TCOMMA typedef_decl_stmt_inner
    {
      VarSymbol* var = new VarSymbol($1);
      var->addFlag(FLAG_TYPE_VARIABLE);
      DefExpr* def = new DefExpr(var, $3);
      $5->insertAtTail(def);
      $$ = buildChapelStmt($5);
    }
;

typedef_decl_stmt:
  TTYPE typedef_decl_stmt_inner TSEMI
    { $$ = $2; }
;


opt_init_type:
  /* nothing */
    { $$ = NULL; }
| TASSIGN type
    { $$ = $2; }
;


typevar_decl_stmt:
  TTYPE identifier opt_init_type TSEMI
    {
      VarSymbol* var = new VarSymbol($2);
      var->addFlag(FLAG_TYPE_VARIABLE);
      DefExpr* def = new DefExpr(var, $3);
      $$ = buildChapelStmt(def);
    }
;


var_decl_stmt:
  is_config TPARAM var_decl_stmt_inner_ls TSEMI
    {
      setVarSymbolAttributes($3, $1, true, false);
      backPropagateInitsTypes($3);
      $$ = $3;
    }
| is_config TCONST var_decl_stmt_inner_ls TSEMI
    {
      setVarSymbolAttributes($3, $1, false, true);
      backPropagateInitsTypes($3);
      $$ = $3;
    }
| is_config TVAR var_decl_stmt_inner_ls TSEMI
    {
      setVarSymbolAttributes($3, $1, false, false);
      backPropagateInitsTypes($3);
      $$ = $3;
    }
;


is_config:
  /* nothing */
    { $$ = false; }
| TCONFIG
    { $$ = true; }
;

 
var_decl_stmt_inner_ls:
  var_decl_stmt_inner
    { $$ = $1; }
| var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner
    {
      for_alist(expr, $3->body)
        $1->insertAtTail(expr->remove());
      $$ = $1;
    }
;


var_decl_stmt_inner:
  identifier opt_type opt_init_expr
    {
      VarSymbol* var = new VarSymbol($1);
      $$ = buildChapelStmt(new DefExpr(var, $3, $2));
    }
| identifier opt_domain alias_expr
    {
      VarSymbol* var = new VarSymbol($1);
      $$ = buildChapelStmt(new DefExpr(var, $3, $2));
      var->addFlag(FLAG_ARRAY_ALIAS);
    }
| TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr
    { $$ = buildTupleVarDeclStmt($2, $4, $5); }
;


tuple_var_decl_stmt_inner_ls:
  identifier
    { $$ = buildChapelStmt(new DefExpr(new VarSymbol($1))); }
| TLP tuple_var_decl_stmt_inner_ls TRP
    { $$ = buildChapelStmt($2); }
| tuple_var_decl_stmt_inner_ls TCOMMA identifier
    {
      $1->insertAtTail(new DefExpr(new VarSymbol($3)));
      $$ = $1;
    }
| tuple_var_decl_stmt_inner_ls TCOMMA TLP tuple_var_decl_stmt_inner_ls TRP
    {
      $1->insertAtTail($4);
      $$ = $1;
    }
;


/** TYPES ********************************************************************/


anon_record_type:
  TRECORD TLCBR class_body_stmt_ls TRCBR
    {
      $$ = buildClassDefExpr(astr("_anon_record", istr(anon_record_uid++)), new ClassType(CLASS_RECORD), $3);
    }
;


distributed_expr: /* not supported in one-locale implementation */
  /* nothing */
    { $$ = new SymExpr("defaultDistribution"); }
| TDISTRIBUTED TLP expr TRP
    { $$ = $3; }
;


array_type:
  TLSBR nonempty_expr_ls TIN expr TRSBR type
    { 
      if ($2->argList.length() != 1)
        USR_FATAL($4, "invalid index expression");
      $$ = new CallExpr("chpl__buildArrayRuntimeType",
                        new CallExpr("chpl__buildDomainExpr", $4), $6, $2->get(1)->remove(),
                        new CallExpr("chpl__buildDomainExpr", $4->copy()));
    }
| TLSBR nonempty_expr_ls TRSBR type
    { $$ = new CallExpr("chpl__buildArrayRuntimeType", new CallExpr("chpl__buildDomainExpr", $2), $4); }
;


type:
  stmt_level_expr
| formal_level_type
;


formal_level_type:
  anon_record_type
| array_type
| TDOMAIN TLP expr_ls TRP distributed_expr
    {
      CallExpr* call = new CallExpr("chpl__buildDomainRuntimeType", $5);
      call->insertAtTail($3);
      $$ = call;
    }
| TSUBDOMAIN TLP expr_ls TRP
    { $$ = new CallExpr("chpl__buildSubDomainType", $3); }
| TSPARSE TSUBDOMAIN TLP expr_ls TRP distributed_expr
    {
      CallExpr* call = new CallExpr("chpl__buildSparseDomainRuntimeType", $6);
      call->insertAtTail($4);
      $$ = call;
    }
| TSINGLE type
    { $$ = new CallExpr( "_singlevar", $2); }
| TSYNC type
    { $$ = new CallExpr( "_syncvar", $2); }
;


alias_expr:
  TALIAS expr
    { $$ = $2; }
;


opt_init_expr:
  /* nothing */
    { $$ = NULL; }
| TASSIGN expr
    { $$ = $2; }
;


opt_domain:
  /* nothing */
    { $$ = NULL; }
| TCOLON TLSBR nonempty_expr_ls TRSBR
    { $$ = new CallExpr("chpl__buildDomainExpr", $3); }
;


opt_type:
  /* nothing */
    { $$ = NULL; }
| TCOLON type
    { $$ = $2; }
;


opt_formal_type:
  /* nothing */
    { $$ = NULL; }
| TCOLON formal_type_expr
    { $$ = $2; }
| TCOLON formal_level_type
    { $$ = $2; }
| TCOLON query_expr
    { $$ = $2; }
| TCOLON TDOMAIN
    { $$ = new SymExpr("_domain"); }
| TCOLON TSINGLE
    { $$ = new SymExpr( "_singlevar"); }
| TCOLON TSYNC
    { $$ = new SymExpr( "_syncvar"); }
| TCOLON TLSBR TRSBR type
    { $$ = new CallExpr("chpl__buildArrayRuntimeType", gNil, $4); }
| TCOLON TLSBR query_expr TRSBR type
    { $$ = new CallExpr("chpl__buildArrayRuntimeType", $3, $5); }
| TCOLON TLSBR TRSBR
    { $$ = new CallExpr("chpl__buildArrayRuntimeType", gNil); }
| TCOLON TLSBR query_expr TRSBR
    { $$ = new CallExpr("chpl__buildArrayRuntimeType", $3); }
| TCOLON TLSBR nonempty_expr_ls TRSBR
    { $$ = new CallExpr("chpl__buildArrayRuntimeType", new CallExpr("chpl__buildDomainExpr", $3)); }
| TCOLON TLSBR TRSBR query_expr
    { $$ = new CallExpr("chpl__buildArrayRuntimeType", gNil, $4); }
| TCOLON TLSBR query_expr TRSBR query_expr
    { $$ = new CallExpr("chpl__buildArrayRuntimeType", $3, $5); }
| TCOLON TLSBR nonempty_expr_ls TRSBR query_expr
    { $$ = new CallExpr("chpl__buildArrayRuntimeType", new CallExpr("chpl__buildDomainExpr", $3), $5); }
;


formal_type_expr:
  formal_tuple_paren_expr
| formal_parenop_expr
| formal_memberaccess_expr
| formal_type_expr TSTAR formal_type_expr
    { $$ = new CallExpr("*", $1, $3); }
| variable_expr
| literal
;


formal_expr_ls:
  /* nothing */
    { $$ = new CallExpr(PRIMITIVE_ACTUALS_LIST); }
| nonempty_formal_expr_ls
;


nonempty_formal_expr_ls:
  formal_expr_list_item
    { $$ = new CallExpr(PRIMITIVE_ACTUALS_LIST, $1); }
| nonempty_formal_expr_ls TCOMMA formal_expr_list_item
    { $1->insertAtTail($3); }
;


formal_expr_list_item:
  identifier TASSIGN formal_expr_list_item
    { $$ = new NamedExpr($1, $3); }
| query_expr
    { $$ = $1; }
| expr
;


formal_tuple_paren_expr:
  TLP nonempty_formal_expr_ls TRP 
    { 
      if ($2->argList.length() == 1) {
        $$ = $2->get(1);
        $$->remove();
      } else {
        $$ = new CallExpr("_build_tuple", $2);
      }
    }
;


formal_parenop_expr:
  formal_type_expr TLP formal_expr_ls TRP
    { $$ = new CallExpr($1, $3); }
| formal_type_expr TLSBR formal_expr_ls TRSBR
    {
      CallExpr* call = new CallExpr($1, $3);
      call->square = true;
      $$ = call;
    }
| TINDEX TLP expr_ls TRP
    { $$ = new CallExpr("chpl__buildIndexType", $3); }
;

formal_memberaccess_expr:
  formal_type_expr TDOT identifier
    { $$ = buildDotExpr($1, $3); }
| formal_type_expr TDOT TTYPE
    { $$ = new CallExpr(PRIMITIVE_TYPEOF, $1); }
| formal_type_expr TDOT TDOMAIN
    { $$ = buildDotExpr($1, "_dom"); }
;


/** EXPRESSIONS **************************************************************/


expr_ls:
  /* nothing */
    { $$ = new CallExpr(PRIMITIVE_ACTUALS_LIST); }
| nonempty_expr_ls
;


nonempty_expr_ls:
  expr_list_item
    { $$ = new CallExpr(PRIMITIVE_ACTUALS_LIST, $1); }
| nonempty_expr_ls TCOMMA expr_list_item
    { $1->insertAtTail($3); }
;


expr_list_item:
  identifier TASSIGN expr
    { $$ = new NamedExpr($1, $3); }
| expr
;


literal:
  INTLITERAL
    {
      unsigned long long int ull;
      if (!strncmp("0b", yytext, 2))
        ull = binStr2uint64(yytext);
      else if (!strncmp("0x", yytext, 2))
        ull = hexStr2uint64(yytext);
      else
        ull = str2uint64(yytext);
      if (ull <= 2147483647ull)
        $$ = new SymExpr(new_IntSymbol(ull, INT_SIZE_32));
      else if (ull <= 9223372036854775807ull)
        $$ = new SymExpr(new_IntSymbol(ull, INT_SIZE_64));
      else
        $$ = new SymExpr(new_UIntSymbol(ull, INT_SIZE_64));
    }
| REALLITERAL
    { $$ = new SymExpr(new_RealSymbol(yytext, strtod(yytext, NULL))); }
| IMAGLITERAL
    {
      yytext[strlen(yytext)-1] = '\0';
      $$ = new SymExpr(new_ImagSymbol(yytext, strtod(yytext, NULL)));
    }
| STRINGLITERAL
    { $$ = new SymExpr(new_StringSymbol($1)); }
;


identifier:
  TIDENT
    { $$ = astr(yytext); }
;


opt_identifier:
  /* nothing */
    { $$ = NULL; }
| identifier
;


tuple_paren_expr:
  TLP nonempty_expr_ls TRP 
    { 
      if ($2->argList.length() == 1) {
        $$ = $2->get(1);
        $$->remove();
      } else {
        $$ = new CallExpr("_build_tuple", $2);
      }
    }
;


parenop_expr:
  lvalue TLP expr_ls TRP
    { $$ = new CallExpr($1, $3); }
| lvalue TLSBR expr_ls TRSBR
    {
      CallExpr* call = new CallExpr($1, $3);
      call->square = true;
      $$ = call;
    }
| TPRIMITIVE TLP expr_ls TRP
    { $$ = buildPrimitiveExpr($3); }
| TCOMPILERERROR TLP expr_ls TRP
    { $$ = new CallExpr(PRIMITIVE_ERROR, $3); }
| TCOMPILERWARNING TLP expr_ls TRP
    { $$ = new CallExpr(PRIMITIVE_WARNING, $3); }
| TINDEX TLP expr_ls TRP
    { $$ = new CallExpr("chpl__buildIndexType", $3); }
;


memberaccess_expr:
  lvalue TDOT identifier
    { $$ = buildDotExpr($1, $3); }
| lvalue TDOT TTYPE
    { $$ = new CallExpr(PRIMITIVE_TYPEOF, $1); }
| lvalue TDOT TDOMAIN
    { $$ = buildDotExpr($1, "_dom"); }
;


variable_expr:
  identifier
    { $$ = new SymExpr($1); }
;


non_tuple_lvalue:
  literal
| variable_expr
| parenop_expr
| memberaccess_expr
| TLSBR nonempty_expr_ls TRSBR
    { $$ = new CallExpr("chpl__buildDomainExpr", $2); }
;


lvalue:
  non_tuple_lvalue
| tuple_paren_expr
;


expr:
  stmt_level_expr
| TLSBR nonempty_expr_ls TIN expr TRSBR expr %prec TRSBR
    {
      if ($2->argList.length() != 1)
        USR_FATAL($4, "invalid index expression");
      $$ = buildForLoopExpr($2->get(1)->remove(), $4, $6);
    }
| TLSBR nonempty_expr_ls TRSBR expr %prec TRSBR
    {
      if ($2->argList.length() != 1)
        USR_FATAL($4, "invalid loop expression");
      $$ = buildForLoopExpr(NULL, $2->get(1)->remove(), $4);
    }
| TLSBR nonempty_expr_ls TIN expr TRSBR TIF expr TTHEN expr %prec TNOELSE
    {
      if ($2->argList.length() != 1)
        USR_FATAL($4, "invalid index expression");
      $$ = buildForLoopExpr($2->get(1)->remove(), $4, $9, $7);
    }
| TLSBR nonempty_expr_ls TRSBR TIF expr TTHEN expr %prec TNOELSE
    {
      if ($2->argList.length() != 1)
        USR_FATAL($5, "invalid loop expression");
      $$ = buildForLoopExpr(NULL, $2->get(1)->remove(), $7, $5);
    }
| TFOR expr TIN expr TDO expr %prec TRSBR
    { $$ = buildForLoopExpr($2, $4, $6); }
| TFOR expr TDO expr %prec TRSBR
    { $$ = buildForLoopExpr(NULL, $2, $4); }
| TFOR expr TIN expr TDO TIF expr TTHEN expr %prec TNOELSE
    { $$ = buildForLoopExpr($2, $4, $9, $7); }
| TFOR expr TDO TIF expr TTHEN expr %prec TNOELSE
    { $$ = buildForLoopExpr(NULL, $2, $7, $5); }
| TFORALL expr TIN expr TDO expr %prec TRSBR
    { $$ = buildForLoopExpr($2, $4, $6); }
| TFORALL expr TDO expr %prec TRSBR
    { $$ = buildForLoopExpr(NULL, $2, $4); }
| TFORALL expr TIN expr TDO TIF expr TTHEN expr %prec TNOELSE
    { $$ = buildForLoopExpr($2, $4, $9, $7); }
| TFORALL expr TDO TIF expr TTHEN expr %prec TNOELSE
    { $$ = buildForLoopExpr(NULL, $2, $7, $5); }
| TIF expr TTHEN expr TELSE expr
    { $$ = new CallExpr(new DefExpr(buildIfExpr($2, $4, $6))); }
;


stmt_level_expr: 
  lvalue
| TNEW parenop_expr
    { $$ = new CallExpr(PRIMITIVE_NEW, $2); }
| TLP TDOTDOTDOT expr TRP
    { $$ = new CallExpr(PRIMITIVE_TUPLE_EXPAND, $3); }
| TNIL
    { $$ = new SymExpr(gNil); }
| TLET var_decl_stmt_inner_ls TIN expr
    { $$ = buildLetExpr($2, $4); }
| reduction %prec TREDUCE
| expr TCOLON expr
    { $$ = new CallExpr("_cast", $3, $1); }
| expr TDOTDOT expr
    { $$ = new CallExpr("_build_range", $1, $3); }
| expr TDOTDOT
    { $$ = new CallExpr("_build_range", buildDotExpr("BoundedRangeType", "boundedLow"), $1); }
| TDOTDOT expr
    { $$ = new CallExpr("_build_range", buildDotExpr("BoundedRangeType", "boundedHigh"), $2); }
| TDOTDOT
    { $$ = new CallExpr("_build_range", buildDotExpr("BoundedRangeType", "boundedNone")); }
| TPLUS expr %prec TUPLUS
    { $$ = new CallExpr("+", $2); }
| TMINUS expr %prec TUMINUS
    { $$ = new CallExpr("-", $2); }
| TMINUSMINUS expr %prec TUMINUS
    { $$ = buildPreDecIncWarning($2, '-'); }
| TPLUSPLUS expr %prec TUPLUS
    { $$ = buildPreDecIncWarning($2, '+'); }
| TNOT expr
    { $$ = new CallExpr("!", $2); }
| TBNOT expr
    { $$ = new CallExpr("~", $2); }
| expr TPLUS expr
    { $$ = new CallExpr("+", $1, $3); }
| expr TMINUS expr
    { $$ = new CallExpr("-", $1, $3); }
| expr TSTAR expr
    { $$ = new CallExpr("*", $1, $3); }
| expr TDIVIDE expr
    { $$ = new CallExpr("/", $1, $3); }
| expr TSHIFTLEFT expr
    { $$ = new CallExpr("<<", $1, $3); }
| expr TSHIFTRIGHT expr
    { $$ = new CallExpr(">>", $1, $3); }
| expr TMOD expr
    { $$ = new CallExpr("%", $1, $3); }
| expr TEQUAL expr
    { $$ = new CallExpr("==", $1, $3); }
| expr TNOTEQUAL expr
    { $$ = new CallExpr("!=", $1, $3); }
| expr TLESSEQUAL expr
    { $$ = new CallExpr("<=", $1, $3); }
| expr TGREATEREQUAL expr
    { $$ = new CallExpr(">=", $1, $3); }
| expr TLESS expr
    { $$ = new CallExpr("<", $1, $3); }
| expr TGREATER expr
    { $$ = new CallExpr(">", $1, $3); }
| expr TBAND expr
    { $$ = new CallExpr("&", $1, $3); }
| expr TBOR expr
    { $$ = new CallExpr("|", $1, $3); }
| expr TBXOR expr
    { $$ = new CallExpr("^", $1, $3); }
| expr TAND expr
    { $$ = buildLogicalAndExpr($1, $3); }
| expr TOR expr
    { $$ = buildLogicalOrExpr($1, $3); }
| expr TEXP expr
    { $$ = new CallExpr("**", $1, $3); }
| expr TBY expr
    { $$ = new CallExpr("by", $1, $3); }
| expr THASH expr
    { $$ = new CallExpr("#", $1, $3); }
;


reduction:
  expr TREDUCE expr
    { $$ = buildReduceScanExpr($1, $3); }
| TPLUS TREDUCE expr
    { $$ = buildReduceScanExpr(new SymExpr("_sum"), $3); }
| TSTAR TREDUCE expr
    { $$ = buildReduceScanExpr(new SymExpr("_prod"), $3); }
| TAND TREDUCE expr
    { $$ = buildReduceScanExpr(new SymExpr("_land"), $3); }
| TOR TREDUCE expr
    { $$ = buildReduceScanExpr(new SymExpr("_lor"), $3); }
| TBAND TREDUCE expr
    { $$ = buildReduceScanExpr(new SymExpr("_band"), $3); }
| TBOR TREDUCE expr
    { $$ = buildReduceScanExpr(new SymExpr("_bor"), $3); }
| TBXOR TREDUCE expr
    { $$ = buildReduceScanExpr(new SymExpr("_bxor"), $3); }
| expr TSCAN expr
    { $$ = buildReduceScanExpr($1, $3, true); }
| TPLUS TSCAN expr
    { $$ = buildReduceScanExpr(new SymExpr("_sum"), $3, true); }
| TSTAR TSCAN expr
    { $$ = buildReduceScanExpr(new SymExpr("_prod"), $3, true); }
| TAND TSCAN expr
    { $$ = buildReduceScanExpr(new SymExpr("_land"), $3, true); }
| TOR TSCAN expr
    { $$ = buildReduceScanExpr(new SymExpr("_lor"), $3, true); }
| TBAND TSCAN expr
    { $$ = buildReduceScanExpr(new SymExpr("_band"), $3, true); }
| TBOR TSCAN expr
    { $$ = buildReduceScanExpr(new SymExpr("_bor"), $3, true); }
| TBXOR TSCAN expr
    { $$ = buildReduceScanExpr(new SymExpr("_bxor"), $3, true); }
;


%%
