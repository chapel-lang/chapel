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

What is parsed_block_stmt?

  In many cases, to facilitate parsing, a block statement or a
statement that would cause no ambiguities if it too were in that
context, are required.  For example,

  function name(formals) : type ...
  if (expr) ...
  while (expr) ...

  Here "..." a block statement is required.  However, some other
statements are also allowed.  These are marked by requiring, instead
of a block statement, a parsed_block_stmt, which includes block_stmt.
Parsed block statements include return statements so for example

  function name(formals) : type return x;

is allowed.

Here is an example of the ambiguity:

  while x(i);

Is this "while x"(i); or "while x(i)";?

***/


%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexyacc.h" // all #includes here, for make depend

  static int anon_record_uid = 1;
  static int iterator_uid = 1;
  static bool atomic_warning = false;
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
  BlockStmt* pblockstmt;

  Type* ptype;
  EnumType* pet;

  Symbol* psym;
  FnSymbol* pfnsym;
  ModuleSymbol* pmodsym;
  EnumSymbol* penumsym;

  AList* palist;
}

%token TATOMIC
%token TBEGIN
%token TBREAK
%token TCLASS
%token TCOBEGIN
%token TCOFORALL
%token TCOMPILERERROR
%token TCONFIG
%token TCONST
%token TCONTINUE
%token TDEF
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
%token TMODULE
%token TNIL
%token TOF
%token TON
%token TOPAQUE
%token TORDERED
%token TOTHERWISE
%token TOUT
%token TPARAM
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
%token TNOTCOLON
%token TQUESTION

%token TLCBR TRCBR
%token TLP TRP
%token TLSBR TRSBR

%token TSEMI

%type <b> is_config
 
%type <pt> formal_tag

%type <retTag> ret_class

%type <pch> identifier fn_identifier opt_identifier
%type <pch> pragma
%type <vpch> pragma_ls

%type <pblockstmt> program stmt_ls
%type <pblockstmt> class_body_stmt_ls

%type <pblockstmt> stmt empty_stmt label_stmt goto_stmt break_stmt continue_stmt
%type <pblockstmt> expr_stmt if_stmt expr_for_stmt for_stmt while_do_stmt do_while_stmt serial_stmt
%type <pblockstmt> select_stmt return_stmt yield_stmt assign_stmt class_body_stmt
%type <pblockstmt> type_select_stmt on_stmt non_empty_stmt use_stmt_ls

%type <pblockstmt> typedef_decl_stmt typedef_decl_stmt_inner fn_decl_stmt class_decl_stmt mod_decl_stmt extern_fn_decl_stmt
%type <pblockstmt> typevar_decl_stmt enum_decl_stmt use_stmt

%type <pblockstmt> var_decl_stmt var_decl_stmt_inner_ls
%type <pblockstmt> var_decl_stmt_inner tuple_var_decl_stmt_inner_ls

%type <pblockstmt> parsed_block_single_stmt

%type <pblockstmt> parsed_block_stmt block_stmt

%type <pexpr> when_stmt
%type <pblockstmt> when_stmt_ls

%type <pexpr> opt_type opt_formal_type array_type opt_elt_type top_level_type
%type <pexpr> type anon_record_type tuple_type type_binding_expr opt_domain
%type <pexpr> composable_type variable_type parenop_type memberaccess_type
%type <ptype> class_tag
%type <pet> enum_ls

%type <pexpr> parenop_expr memberaccess_expr non_tuple_lvalue lvalue
%type <pexpr> tuple_paren_expr expr expr_list_item opt_expr
%type <pexpr> literal where distributed_expr
%type <pexpr> variable_expr top_level_expr alias_expr
%type <pexpr> reduction opt_init_expr opt_init_type var_arg_expr type_expr
%type <palist> expr_ls nonempty_expr_ls opt_inherit_expr_ls type_ls type_expr_ls
%type <pdefexpr> formal enum_item

%type <pfnsym> fn_decl_stmt_inner formal_ls opt_formal_ls

/* These are declared in increasing order of precedence. */

%left TNOELSE
%left TELSE
%left TCOMMA
%left TREDUCE TSCAN
%left TFOR
%left TFORALL
%left TIF
%left TBY
%left TRSBR
%left TIN
%left TDOTDOT
%right TSTARTUPLE
%left TOR
%left TAND
%right TNOT
%left TEQUAL TNOTEQUAL
%left TLESSEQUAL TGREATEREQUAL TLESS TGREATER
%left TBOR
%left TBXOR
%left TBAND
%left TSHIFTLEFT TSHIFTRIGHT
%left TPLUS TMINUS
%left TSTAR TDIVIDE TMOD
%left TCOLON TNOTCOLON
%right TUPLUS TUMINUS TBNOT
%right TEXP
%left TLP TLSBR
%left TDOT

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
      $3->body.first()->addPragmas($2);
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
| for_stmt
| expr_for_stmt
| while_do_stmt
| do_while_stmt
| select_stmt
| type_select_stmt
| return_stmt
| yield_stmt
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
| serial_stmt
| error
    { printf("syntax error"); exit(1); }
;


parsed_block_single_stmt:
  empty_stmt
| label_stmt
| goto_stmt
| break_stmt
| continue_stmt
| if_stmt
| select_stmt
| for_stmt
| while_do_stmt
| return_stmt
| yield_stmt
| on_stmt
| serial_stmt
;


parsed_block_stmt:
  parsed_block_single_stmt
    { $$ = new BlockStmt($1); }
| block_stmt
;


empty_stmt:
  TSEMI
    { $$ = build_chpl_stmt(new BlockStmt()); }
;


label_stmt:
  TLABEL identifier stmt
    {
      $$ = build_chpl_stmt($3);
      $$->insertAtTail(buildLabelStmt(astr("_post", $2)));
      $$->insertAtHead(buildLabelStmt($2));
    }
;


goto_stmt:
  TGOTO identifier TSEMI
    { $$ = build_chpl_stmt(new GotoStmt(GOTO_NORMAL, $2)); }
;


break_stmt:
  TBREAK opt_identifier TSEMI
    { $$ = build_chpl_stmt(new GotoStmt(GOTO_BREAK, $2)); }
;


continue_stmt:
  TCONTINUE opt_identifier TSEMI
    { $$ = build_chpl_stmt(new GotoStmt(GOTO_CONTINUE, $2)); }
;


expr_stmt:
  top_level_expr TSEMI
    { $$ = build_chpl_stmt($1); }
;


if_stmt:
  TIF expr parsed_block_stmt            %prec TNOELSE
    { $$ = build_chpl_stmt(new CondStmt(new CallExpr("_cond_test", $2), $3)); }
| TIF expr TTHEN stmt                   %prec TNOELSE
    { $$ = build_chpl_stmt(new CondStmt(new CallExpr("_cond_test", $2), $4)); }
| TIF expr parsed_block_stmt TELSE stmt
    { $$ = build_chpl_stmt(new CondStmt(new CallExpr("_cond_test", $2), $3, $5)); }
| TIF expr TTHEN stmt TELSE stmt
    { $$ = build_chpl_stmt(new CondStmt(new CallExpr("_cond_test", $2), $4, $6)); }
;


for_stmt:
  TFOR TPARAM identifier TIN expr TDOTDOT expr TDO stmt
    { $$ = build_param_for_stmt($3, $5, $7, new SymExpr(new_IntSymbol(1)), $9); }
| TFOR TPARAM identifier TIN expr TDOTDOT expr parsed_block_stmt
    { $$ = build_param_for_stmt($3, $5, $7, new SymExpr(new_IntSymbol(1)), $8); }
/*
| TFOR TPARAM identifier TIN expr TDOTDOT expr TBY expr TDO stmt
    { $$ = build_param_for_stmt($3, $5, $7, $9, $11); }
| TFOR TPARAM identifier TIN expr TDOTDOT expr TBY expr parsed_block_stmt
    { $$ = build_param_for_stmt($3, $5, $7, $9, $10); }
*/

| TFOR expr TIN expr parsed_block_stmt
    { $$ = build_for_block(BLOCK_FOR, $2, $4, $5); }
| TFOR expr TIN expr TDO stmt
    { $$ = build_for_block(BLOCK_FOR, $2, $4, new BlockStmt($6)); }
| TFOR expr parsed_block_stmt
    { $$ = build_for_block(BLOCK_FOR, new SymExpr("_dummy"), $2, $3); }
| TFOR expr TDO stmt
    { $$ = build_for_block(BLOCK_FOR, new SymExpr("_dummy"), $2, new BlockStmt($4)); }

| TFORALL expr TIN expr parsed_block_stmt
    { $$ = build_for_block((fSerial) ? BLOCK_FOR : BLOCK_FORALL, $2, $4, $5); }
| TFORALL expr TIN expr TDO stmt
    { $$ = build_for_block((fSerial) ? BLOCK_FOR : BLOCK_FORALL, $2, $4, new BlockStmt($6)); }
| TFORALL expr parsed_block_stmt
    { $$ = build_for_block((fSerial) ? BLOCK_FOR : BLOCK_FORALL, new SymExpr("_dummy"), $2, $3); }
| TFORALL expr TDO stmt
    { $$ = build_for_block((fSerial) ? BLOCK_FOR : BLOCK_FORALL, new SymExpr("_dummy"), $2, new BlockStmt($4)); }

| TORDERED TFORALL expr TIN expr parsed_block_stmt
    { $$ = build_for_block((fSerial) ? BLOCK_FOR : BLOCK_ORDERED_FORALL, $3, $5, $6); }
| TORDERED TFORALL expr TIN expr TDO stmt
    { $$ = build_for_block((fSerial) ? BLOCK_FOR : BLOCK_ORDERED_FORALL, $3, $5, new BlockStmt($7)); }
| TORDERED TFORALL expr parsed_block_stmt
    { $$ = build_for_block((fSerial) ? BLOCK_FOR : BLOCK_ORDERED_FORALL, new SymExpr("_dummy"), $3, $4); }
| TORDERED TFORALL expr TDO stmt
    { $$ = build_for_block((fSerial) ? BLOCK_FOR : BLOCK_ORDERED_FORALL, new SymExpr("_dummy"), $3, new BlockStmt($5)); }

| TCOFORALL expr TIN expr parsed_block_stmt
    { $$ = build_for_block((fSerial) ? BLOCK_FOR : BLOCK_COFORALL, $2, $4, $5); }
| TCOFORALL expr TIN expr TDO stmt
    { $$ = build_for_block((fSerial) ? BLOCK_FOR : BLOCK_COFORALL, $2, $4, new BlockStmt($6)); }
| TCOFORALL expr parsed_block_stmt
    { $$ = build_for_block((fSerial) ? BLOCK_FOR : BLOCK_COFORALL, new SymExpr("_dummy"), $2, $3); }
| TCOFORALL expr TDO stmt
    { $$ = build_for_block((fSerial) ? BLOCK_FOR : BLOCK_COFORALL, new SymExpr("_dummy"), $2, new BlockStmt($4)); }
;


expr_for_stmt:
  TLSBR nonempty_expr_ls TIN expr TRSBR stmt
    {
      if ($2->length() != 1)
        USR_FATAL($4, "invalid index expression");
      $$ = build_for_block(BLOCK_FORALL, $2->only()->remove(), $4, new BlockStmt($6));
    }
| TLSBR nonempty_expr_ls TRSBR non_empty_stmt
    {
      $$ = build_for_block(BLOCK_FORALL, new SymExpr("_dummy"), $2->only()->remove(), new BlockStmt($4));
    }
;


while_do_stmt:
  TWHILE expr TDO stmt
    { $$ = build_while_do_block(new CallExpr("_cond_test", $2), new BlockStmt($4)); }
| TWHILE expr parsed_block_stmt
    { $$ = build_while_do_block(new CallExpr("_cond_test", $2), $3); }
;


do_while_stmt:
TDO stmt TWHILE expr TSEMI
    { $$ = build_do_while_block(new CallExpr("_cond_test", $4), $2); }
;


type_select_stmt:
  TTYPE TSELECT nonempty_expr_ls TLCBR when_stmt_ls TRCBR
    { $$ = build_type_select($3, $5); }
;


select_stmt:
  TSELECT expr TLCBR when_stmt_ls TRCBR
    { $$ = build_chpl_stmt(build_select($2, $4)); }
;


when_stmt_ls:
  /* nothing */
    { $$ = build_chpl_stmt(); }
| when_stmt_ls when_stmt
    { $1->insertAtTail($2); }
; 


when_stmt:
  TWHEN nonempty_expr_ls TDO stmt
    { $$ = new CondStmt(new CallExpr(PRIMITIVE_WHEN, $2), $4); }
| TWHEN nonempty_expr_ls parsed_block_stmt
    { $$ = new CondStmt(new CallExpr(PRIMITIVE_WHEN, $2), $3); }
| TOTHERWISE stmt
    { $$ = new CondStmt(new CallExpr(PRIMITIVE_WHEN), $2); }
;


return_stmt:
  TRETURN opt_expr TSEMI
    { $$ = build_chpl_stmt(new CallExpr(PRIMITIVE_RETURN, $2)); }
;


yield_stmt:
  TYIELD opt_expr TSEMI
    { $$ = build_chpl_stmt(new CallExpr(PRIMITIVE_YIELD, $2)); }
;


assign_stmt:
  lvalue TASSIGN expr TSEMI
    {
      $$ = build_chpl_stmt(new CallExpr("=", $1, $3));
    }
| lvalue TASSIGNPLUS expr TSEMI
    {
      $$ = buildCompoundAssignment("+", $1, $3);
    }
| lvalue TASSIGNMINUS expr TSEMI
    {
      $$ = buildCompoundAssignment("-", $1, $3);
    }
| lvalue TASSIGNMULTIPLY expr TSEMI
    {
      $$ = buildCompoundAssignment("*", $1, $3);
    }
| lvalue TASSIGNDIVIDE expr TSEMI
    {
      $$ = buildCompoundAssignment("/", $1, $3);
    }
| lvalue TASSIGNMOD expr TSEMI
    {
      $$ = buildCompoundAssignment("%", $1, $3);
    }
| lvalue TASSIGNEXP expr TSEMI
    {
      $$ = buildCompoundAssignment("**", $1, $3);
    }
| lvalue TASSIGNBAND expr TSEMI
    {
      $$ = buildCompoundAssignment("&", $1, $3);
    }
| lvalue TASSIGNBOR expr TSEMI
    {
      $$ = buildCompoundAssignment("|", $1, $3);
    }
| lvalue TASSIGNBXOR expr TSEMI
    {
      $$ = buildCompoundAssignment("^", $1, $3);
    }
| lvalue TASSIGNLAND expr TSEMI
    {
      $$ = buildLogicalAndAssignment($1, $3);
    }
| lvalue TASSIGNLOR expr TSEMI
    {
      $$ = buildLogicalOrAssignment($1, $3);
    }
| lvalue TASSIGNSR expr TSEMI
    {
      $$ = buildCompoundAssignment(">>", $1, $3);
    }
| lvalue TASSIGNSL expr TSEMI
    {
      $$ = buildCompoundAssignment("<<", $1, $3);
    }
| lvalue TSWAP expr TSEMI
    {
      $$ = build_chpl_stmt(new CallExpr("_chpl_swap", $1, $3));
    }
;


block_stmt:
  TLCBR stmt_ls TRCBR
    {
      $$ = build_chpl_stmt($2);
    }
| TCOBEGIN TLCBR stmt_ls TRCBR
    {
      if (fSerial)
        $$ = build_chpl_stmt($3);
      else {
        $3->blockTag = BLOCK_COBEGIN;
        $$ = build_chpl_stmt($3);
      }
    }
| TATOMIC stmt
    {
      if (!atomic_warning) {
        atomic_warning = true;
        USR_WARN($2, "atomic keyword is ignored (not implemented)");
      }
      if (fSerial)
        $$ = build_chpl_stmt(new BlockStmt($2, BLOCK_NORMAL));
      else
        $$ = build_chpl_stmt(new BlockStmt($2, BLOCK_ATOMIC));
    }
| TBEGIN stmt
    {
      if (fSerial)
        $$ = build_chpl_stmt(new BlockStmt($2, BLOCK_NORMAL));
      else
        $$ = build_chpl_stmt(new BlockStmt($2, BLOCK_BEGIN));
    }
;


on_stmt:
  TON expr TDO stmt
    {
      if (fLocal)
        $$ = build_chpl_stmt(new BlockStmt($4, BLOCK_NORMAL));
      else
        $$ = build_chpl_stmt(buildOnStmt($2, $4));
    }
| TON expr parsed_block_stmt
    {
      if (fLocal)
        $$ = build_chpl_stmt(new BlockStmt($3, BLOCK_NORMAL));
      else
        $$ = build_chpl_stmt(buildOnStmt($2, $3));
    }
;


serial_stmt:
  TSERIAL expr parsed_block_stmt
    {
      if (fSerial) {
        $3->insertAtHead(new CallExpr("_cond_test", $2));
        $$ = $3;
      } else
        $$ = build_serial_block(new CallExpr("_cond_test", $2), $3);
    }
;



/** DECLARATION STATEMENTS ***************************************************/


class_body_stmt_ls:
  /* nothing */
    { $$ = new BlockStmt(); }
| class_body_stmt_ls pragma_ls class_body_stmt
    {
      $3->body.first()->addPragmas($2);
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


use_stmt:
  TUSE use_stmt_ls TSEMI
    { $$ = $2; }
;


use_stmt_ls:
  lvalue
    { $$ = build_chpl_stmt(new CallExpr(PRIMITIVE_USE, $1)); }
| use_stmt_ls TCOMMA lvalue
    {
      $1->insertAtTail(new CallExpr(PRIMITIVE_USE, $3));
      $$ = $1;
    }
;


mod_decl_stmt:
  TMODULE identifier TLCBR stmt_ls TRCBR
    {
      $$ = build_chpl_stmt(new DefExpr(build_module($2, MOD_USER, $4)));
    }
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
      build_tuple_arg($$, $2, NULL);
    }
| formal_ls TCOMMA formal
    { $1->insertFormalAtTail($3); }
| formal_ls TCOMMA TLP tuple_var_decl_stmt_inner_ls TRP
    { build_tuple_arg($1, $4, NULL); }
;


opt_formal_ls:
  { $$ = new FnSymbol("_"); $$->noParens = true; }
| TLP formal_ls TRP
  { $$ = $2; }
;


fn_decl_stmt_inner:
  fn_identifier opt_formal_ls
    { $$ = $2; $$->name = astr($1); $$->cname = $$->name; }
| type_binding_expr TDOT identifier opt_formal_ls
    {
      $$ = $4;
      $$->name = astr($3);
      $$->cname = $$->name;
      $$->_this = new ArgSymbol(INTENT_BLANK, "this", dtUnknown);
      $$->insertFormalAtHead(new DefExpr($$->_this, NULL, $1));
      $$->insertFormalAtHead(new DefExpr(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken)));
    }
;


ret_class:
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
  ret_class opt_type where parsed_block_stmt
    {
      $3->retTag = $5;
      if ($5 == RET_VAR)
        $3->setter = new DefExpr(new ArgSymbol(INTENT_BLANK, "setter", dtBool));
      $3->retExprType = $6;
      if ($7)
        $3->where = new BlockStmt($7);
      $3->insertAtTail($8);
      $$ = build_chpl_stmt(new DefExpr($3));
    }
;


extern_fn_decl_stmt:
  TEXTERN TDEF fn_decl_stmt_inner opt_type TSEMI
    {
      FnSymbol* fn = $3;
      fn->isExtern = true;
      if ($4)
        fn->retExprType = $4;
      else
        fn->retType = dtVoid;
      $$ = build_chpl_stmt(new DefExpr(fn));
    }
;


var_arg_expr:
  TDOTDOTDOT expr
    { $$ = $2; }
| TDOTDOTDOT TQUESTION identifier
    {
      VarSymbol* var = new VarSymbol($3, dtUnknown);
      var->isParam = true;
      $$ = new DefExpr(var);
    }
;


formal:
  formal_tag pragma_ls identifier opt_formal_type opt_init_expr
    {
      $$ = build_arg($1, $3, $4, $5, NULL);
      $$->sym->addPragmas($2);
      delete $2;
    }
| formal_tag pragma_ls identifier opt_formal_type var_arg_expr
    {
      $$ = build_arg($1, $3, $4, NULL, $5);
      $$->sym->addPragmas($2);
      delete $2;
    }
;


fn_identifier:
  identifier
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
;


where:
  /* nothing */
    { $$ = NULL; }
| TWHERE expr
    { $$ = $2; }
;


type_binding_expr:
  variable_expr
  /* | parenop_expr */;


class_decl_stmt:
  class_tag pragma_ls identifier opt_inherit_expr_ls TLCBR class_body_stmt_ls TRCBR
    {
      DefExpr* def = build_class($3, $1, $6);
      def->sym->addPragmas($2);
      delete $2;
      toClassType(toTypeSymbol(def->sym)->type)->inherits.insertAtTail($4);
      $$ = build_chpl_stmt(def);
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
    { $$ = new AList(); }
| TCOLON nonempty_expr_ls
    { $$ = $2; }
;


enum_decl_stmt:
  TENUM pragma_ls identifier TLCBR enum_ls TRCBR TSEMI
    {
      EnumType* pdt = $5;
      TypeSymbol* pst = new TypeSymbol($3, pdt);
      $5->symbol = pst;
      pst->addPragmas($2);
      delete $2;
      $$ = build_chpl_stmt(new DefExpr(pst));
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
  pragma_ls identifier TASSIGN top_level_type
    {
      UserType* newtype = new UserType($4);
      TypeSymbol* typeSym = new TypeSymbol($2, newtype);
      typeSym->addPragmas($1);
      delete $1;
      DefExpr* def_expr = new DefExpr(typeSym);
      $$ = build_chpl_stmt(def_expr);
    }
| pragma_ls identifier TASSIGN top_level_type TCOMMA typedef_decl_stmt_inner
    {
      UserType* newtype = new UserType($4);
      TypeSymbol* typeSym = new TypeSymbol($2, newtype);
      typeSym->addPragmas($1);
      delete $1;
      DefExpr* def_expr = new DefExpr(typeSym);
      $6->insertAtTail(def_expr);
      $$ = $6;
    }
;

typedef_decl_stmt:
  TTYPE typedef_decl_stmt_inner TSEMI
    { $$ = $2; }
;


opt_init_type:
  /* nothing */
    { $$ = NULL; }
| TASSIGN top_level_type
    { $$ = $2; }
;


typevar_decl_stmt:
  TTYPE pragma_ls identifier opt_init_type TSEMI
    {
      VarSymbol* var = new VarSymbol($3);
      var->isTypeVariable = true;
      var->addPragmas($2);
      delete $2;
      DefExpr* def = new DefExpr(var, $4);
      $$ = build_chpl_stmt(def);
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
      $$ = build_chpl_stmt(new DefExpr(var, $3, $2));
    }
| identifier opt_domain alias_expr
    {
      VarSymbol* var = new VarSymbol($1);
      $$ = build_chpl_stmt(new DefExpr(var, $3, $2));
      var->isUserAlias = true;
    }
| TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr
    {
      VarSymbol* tmp = new VarSymbol("_tuple_tmp");
      tmp->isCompilerTemp = true;
      int count = 1;
      for_alist(expr, $2->body) {
        if (DefExpr* def = toDefExpr(expr)) {
          if (strcmp(def->sym->name, "_")) {
            def->init = new CallExpr(tmp, new_IntSymbol(count));
          } else {
            def->remove();
          }
        } else if (BlockStmt* blk = toBlockStmt(expr)) {
          build_tuple_var_decl(new CallExpr(tmp, new_IntSymbol(count)),
                               blk, expr);
        }
        count++;
      }
      $2->insertAtHead(new DefExpr(tmp, $5, $4));
      $$ = $2;
    }
;


tuple_var_decl_stmt_inner_ls:
  identifier
    {
      $$ = build_chpl_stmt(new DefExpr(new VarSymbol($1)));
    }
| TLP tuple_var_decl_stmt_inner_ls TRP
    { 
      $$ = build_chpl_stmt($2);
    }
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


type_ls:
  type
    { $$ = new AList($1); }
| type_ls TCOMMA type
    { $1->insertAtTail($3); }
;


tuple_type:
  TLP type_ls TRP
    {
      CallExpr* call = new CallExpr("_tuple", new_IntSymbol($2->length()));
      for_alist(expr, *$2) {
        call->insertAtTail(new CallExpr("_init", expr->remove()));
      }
      $$ = call;
    }
;


anon_record_type:
  TRECORD TLCBR class_body_stmt_ls TRCBR
    {
      $$ = build_class(astr("_anon_record", istr(anon_record_uid++)), new ClassType(CLASS_RECORD), $3);
    }
;


variable_type:
  identifier
    { $$ = new SymExpr(new UnresolvedSymbol($1)); }
;


type_expr:
  expr_list_item
| TQUESTION identifier
    { $$ = new DefExpr(new VarSymbol($2)); }
| identifier TASSIGN TQUESTION identifier
    { $$ = new NamedExpr($1, new DefExpr(new VarSymbol($4))); }
;


type_expr_ls:
  /* nothing */
    { $$ = new AList(); }
| type_expr
    { $$ = new AList($1); }
| type_expr_ls TCOMMA type_expr
    { $1->insertAtTail($3); }
;


parenop_type:
  composable_type TLP type_expr_ls TRP
    { $$ = new CallExpr($1, $3); }
;


memberaccess_type:
  composable_type TDOT identifier
    { $$ = new CallExpr(".", $1, new_StringSymbol($3)); }
| composable_type TDOT TTYPE
    { $$ = new CallExpr(PRIMITIVE_TYPEOF, $1); }
| composable_type TDOT TDOMAIN
    { $$ = new CallExpr(".", $1, new_StringSymbol("dom")); }
;


composable_type:
  literal
| variable_type
| parenop_type
| memberaccess_type
;


distributed_expr: /* not supported in one-locale implementation */
  /* nothing */
    { $$ = new CallExpr(defaultDistribution); }
| TDISTRIBUTED TLP expr TRP
    { $$ = $3; }
;


opt_elt_type:
  /* nothing */
    { $$ = NULL; }
| type
    { $$ = $1; }
;


array_type:
  TLSBR nonempty_expr_ls TRSBR opt_elt_type
    { $$ = new CallExpr("_build_array_type", new CallExpr("_build_domain", $2), $4); }
| TLSBR nonempty_expr_ls TIN expr TRSBR type
    { 
      if ($2->length() != 1)
        USR_FATAL($4, "invalid index expression");
      $$ = new CallExpr("_build_array_type",
                        new CallExpr("_build_domain", $4), $6, $2->only()->remove(),
                        new CallExpr("_build_domain", $4->copy()));
    }
| TLSBR TRSBR opt_elt_type
    { $$ = new CallExpr("_build_array_type", gNil, $3); }
| TLSBR TQUESTION identifier TRSBR opt_elt_type
    { $$ = new CallExpr("_build_array_type", new DefExpr(new VarSymbol($3)), $5); }
;


top_level_type:
  type
| composable_type TSTAR type %prec TSTAR  
    { $$ = new CallExpr("_tuple", $1, new CallExpr("_init", $3)); }
;


type:
  composable_type %prec TSTARTUPLE
| anon_record_type
| tuple_type
| composable_type TOF type  
    { $$ = new CallExpr($1, new NamedExpr("eltType", $3)); }
| array_type
| TDOMAIN TLP expr_ls TRP distributed_expr
    {
      CallExpr* call = new CallExpr("_build_domain_type", $5);
      call->insertAtTail($3);
      $$ = call;
    }
| TSUBDOMAIN TLP expr_ls TRP
    { $$ = new CallExpr("_build_subdomain_type", $3); }
| TDOMAIN
    { $$ = new SymExpr("_domain"); }
| TDOMAIN TLP TOPAQUE TRP distributed_expr
    { $$ = new CallExpr("_build_opaque_domain_type", $5); }
| TSPARSE TSUBDOMAIN TLP expr_ls TRP distributed_expr
    {
      CallExpr* call = new CallExpr("_build_sparse_subdomain_type", $6);
      call->insertAtTail($4);
      $$ = call;
    }
| TINDEX TLP expr_ls TRP
    { $$ = new CallExpr("_build_index_type", $3); }
| TINDEX TLP TOPAQUE TRP
    { $$ = new SymExpr("_OpaqueIndex"); }
| TSINGLE type
    { $$ = new CallExpr( "_singlevar", $2); }
| TSYNC type
    { $$ = new CallExpr( "_syncvar", $2); }
| TPRIMITIVE TLP expr_ls TRP
    {
      $$ = build_primitive_call($3);
    }
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
    { $$ = new CallExpr("_build_domain", $3); }
;


opt_type:
  /* nothing */
    { $$ = NULL; }
| TCOLON top_level_type
    { $$ = $2; }
;


opt_formal_type:
  opt_type
| TCOLON TQUESTION identifier
    {
      $$ = new DefExpr(new VarSymbol($3));
    }
;


/** EXPRESSIONS **************************************************************/


expr_ls:
  /* nothing */
    { $$ = new AList(); }
| nonempty_expr_ls
;


nonempty_expr_ls:
  pragma_ls expr_list_item
    { $2->addPragmas($1); delete $1; $$ = new AList($2); }
| nonempty_expr_ls TCOMMA pragma_ls expr_list_item
    { $4->addPragmas($3); delete $3; $1->insertAtTail($4); }
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
      if ($2->length() == 1) {
        $$ = $2->get(1);
        $$->remove();
      } else {
        CallExpr* tupleCall = new CallExpr("_tuple", $2);
        tupleCall->insertAtHead(new_IntSymbol(tupleCall->numActuals()));
        $$ = tupleCall;
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
    {
      $$ = build_primitive_call($3);
    }
| TCOMPILERERROR TLP expr_ls TRP
    {
      $$ = new CallExpr(PRIMITIVE_ERROR, $3);
    }
;


memberaccess_expr:
  lvalue TDOT identifier
    { $$ = new CallExpr(".", $1, new_StringSymbol($3)); }
| lvalue TDOT TTYPE
    { $$ = new CallExpr(PRIMITIVE_TYPEOF, $1); }
| lvalue TDOT TDOMAIN
    { $$ = new CallExpr(".", $1, new_StringSymbol("_dom")); }
;


variable_expr:
  identifier
    { $$ = new SymExpr(new UnresolvedSymbol($1)); }
;


non_tuple_lvalue:
  literal
| variable_expr
| parenop_expr
| memberaccess_expr
| TLSBR nonempty_expr_ls TRSBR
    { $$ = new CallExpr("_build_domain", $2); }
| TLSBR nonempty_expr_ls TRP
    {
      if ($2->length() == 1) {
        $$ = new CallExpr("_build_open_interval_upper", $2);
      } else {
        $$ = new CallExpr("_build_open_interval_upper",
                          new CallExpr("_build_domain", $2));
      }
    }
;


lvalue:
  non_tuple_lvalue
| tuple_paren_expr
;


opt_expr:
  /* nothing */
    { $$ = new SymExpr(gVoid); }
| expr
;


expr:
  top_level_expr
| TLSBR nonempty_expr_ls TIN expr TRSBR expr %prec TRSBR
    {
      if ($2->length() != 1)
        USR_FATAL($4, "invalid index expression");
      FnSymbol* forall_iterator =
        new FnSymbol(astr("_forallexpr", istr(iterator_uid++)));
      forall_iterator->fnTag = FN_ITERATOR;
      forall_iterator->insertAtTail(build_for_expr($2->only()->remove(), $4, $6));
      $$ = new CallExpr(new DefExpr(forall_iterator));
    }
| TLSBR nonempty_expr_ls TRSBR expr %prec TRSBR
    {
      if ($2->length() != 1)
        USR_FATAL($4, "invalid loop expression");
      FnSymbol* forall_iterator =
        new FnSymbol(astr("_forallexpr", istr(iterator_uid++)));
      forall_iterator->insertAtTail(build_for_expr(new SymExpr("_dummy"), $2->only()->remove(), $4));
      $$ = new CallExpr(new DefExpr(forall_iterator));
    }
| TLSBR nonempty_expr_ls TIN expr TRSBR TIF expr TTHEN expr %prec TNOELSE
    {
      if ($2->length() != 1)
        USR_FATAL($4, "invalid index expression");
      FnSymbol* forif_fn = new FnSymbol(astr("_forif_fn", istr(iterator_uid++)));
      forif_fn->insertAtTail(build_for_expr($2->only()->remove(), $4, $9, new CallExpr("_cond_test", $7)));
      $$ = new CallExpr(new DefExpr(forif_fn));
    }
| TLSBR nonempty_expr_ls TRSBR TIF expr TTHEN expr %prec TNOELSE
    {
      if ($2->length() != 1)
        USR_FATAL($5, "invalid index expression");
      FnSymbol* forif_fn = new FnSymbol(astr("_forif_fn", istr(iterator_uid++)));
      forif_fn->insertAtTail(build_for_expr(new SymExpr("_dummy"), $2->only()->remove(), $7, new CallExpr("_cond_test", $5)));
      $$ = new CallExpr(new DefExpr(forif_fn));
    }
| TFOR expr TIN expr TDO expr %prec TRSBR
    {
      FnSymbol* forall_iterator =
        new FnSymbol(astr("_forallexpr", istr(iterator_uid++)));
      forall_iterator->fnTag = FN_ITERATOR;
      forall_iterator->insertAtTail(build_for_expr($2, $4, $6));
      $$ = new CallExpr(new DefExpr(forall_iterator));
    }
| TFOR expr TDO expr %prec TRSBR
    {
      FnSymbol* forall_iterator =
        new FnSymbol(astr("_forallexpr", istr(iterator_uid++)));
      forall_iterator->insertAtTail(build_for_expr(new SymExpr("_dummy"), $2, $4));
      $$ = new CallExpr(new DefExpr(forall_iterator));
    }
| TFOR expr TIN expr TDO TIF expr TTHEN expr %prec TNOELSE
    {
      FnSymbol* forif_fn = new FnSymbol(astr("_forif_fn", istr(iterator_uid++)));
      forif_fn->insertAtTail(build_for_expr($2, $4, $9, new CallExpr("_cond_test", $7)));
      $$ = new CallExpr(new DefExpr(forif_fn));
    }
| TFOR expr TDO TIF expr TTHEN expr %prec TNOELSE
    {
      FnSymbol* forif_fn = new FnSymbol(astr("_forif_fn", istr(iterator_uid++)));
      forif_fn->insertAtTail(build_for_expr(new SymExpr("_dummy"), $2, $7, new CallExpr("_cond_test", $5)));
      $$ = new CallExpr(new DefExpr(forif_fn));
    }
| TFORALL expr TIN expr TDO expr %prec TRSBR
    {
      FnSymbol* forall_iterator =
        new FnSymbol(astr("_forallexpr", istr(iterator_uid++)));
      forall_iterator->fnTag = FN_ITERATOR;
      forall_iterator->insertAtTail(build_for_expr($2, $4, $6));
      $$ = new CallExpr(new DefExpr(forall_iterator));
    }
| TFORALL expr TDO expr %prec TRSBR
    {
      FnSymbol* forall_iterator =
        new FnSymbol(astr("_forallexpr", istr(iterator_uid++)));
      forall_iterator->insertAtTail(build_for_expr(new SymExpr("_dummy"), $2, $4));
      $$ = new CallExpr(new DefExpr(forall_iterator));
    }
| TFORALL expr TIN expr TDO TIF expr TTHEN expr %prec TNOELSE
    {
      FnSymbol* forif_fn = new FnSymbol(astr("_forif_fn", istr(iterator_uid++)));
      forif_fn->insertAtTail(build_for_expr($2, $4, $9, new CallExpr("_cond_test", $7)));
      $$ = new CallExpr(new DefExpr(forif_fn));
    }
| TFORALL expr TDO TIF expr TTHEN expr %prec TNOELSE
    {
      FnSymbol* forif_fn = new FnSymbol(astr("_forif_fn", istr(iterator_uid++)));
      forif_fn->insertAtTail(build_for_expr(new SymExpr("_dummy"), $2, $7, new CallExpr("_cond_test", $5)));
      $$ = new CallExpr(new DefExpr(forif_fn));
    }
| TIF expr TTHEN expr TELSE expr
    { $$ = new CallExpr(new DefExpr(build_if_expr(new CallExpr("_cond_test", $2), $4, $6))); }
;


top_level_expr: 
  lvalue
| TLP TDOTDOTDOT expr TRP
    { $$ = new CallExpr(PRIMITIVE_TUPLE_EXPAND, $3); }
| TNIL
    { $$ = new SymExpr(gNil); }
| TLET var_decl_stmt_inner_ls TIN expr
    { $$ = new CallExpr(new DefExpr(build_let_expr($2, $4))); }
| TINDEX TLP expr_ls TRP
    { $$ = new CallExpr("_build_index_type", $3); }
| reduction %prec TREDUCE
| expr TCOLON type
    { $$ = new CallExpr("_cast", $3, $1); }
| expr TDOTDOT expr
    { $$ = new CallExpr("_build_range", $1, $3); }
| expr TDOTDOT
    { $$ = new CallExpr("_build_range", new UnresolvedSymbol("boundedLow"), $1); }
| TDOTDOT expr
    { $$ = new CallExpr("_build_range", new UnresolvedSymbol("boundedHigh"), $2); }
| TDOTDOT
    { $$ = new CallExpr("_build_range", new UnresolvedSymbol("boundedNone")); }
| TPLUS expr %prec TUPLUS
    { $$ = new CallExpr("+", $2); }
| TMINUS expr %prec TUMINUS
    { $$ = new CallExpr("-", $2); }
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
    { $$ = buildLogicalAnd($1, $3); }
| expr TOR expr
    { $$ = buildLogicalOr($1, $3); }
| expr TEXP expr
    { $$ = new CallExpr("**", $1, $3); }
| expr TBY expr
    { $$ = new CallExpr("by", $1, $3); }
;

reduction:
  expr TREDUCE expr
    { $$ = new CallExpr(new DefExpr(build_reduce($1, $3))); }
| TPLUS TREDUCE expr
    { $$ = new CallExpr(new DefExpr(build_reduce(new SymExpr("_sum"), $3))); }
| TSTAR TREDUCE expr
    { $$ = new CallExpr(new DefExpr(build_reduce(new SymExpr("_prod"), $3))); }
| TAND TREDUCE expr
    { $$ = new CallExpr(new DefExpr(build_reduce(new SymExpr("_land"), $3))); }
| TOR TREDUCE expr
    { $$ = new CallExpr(new DefExpr(build_reduce(new SymExpr("_lor"), $3))); }
| TBAND TREDUCE expr
    { $$ = new CallExpr(new DefExpr(build_reduce(new SymExpr("_band"), $3))); }
| TBOR TREDUCE expr
    { $$ = new CallExpr(new DefExpr(build_reduce(new SymExpr("_bor"), $3))); }
| TBXOR TREDUCE expr
    { $$ = new CallExpr(new DefExpr(build_reduce(new SymExpr("_bxor"), $3))); }
|  expr TSCAN expr
    { $$ = new CallExpr(new DefExpr(build_reduce($1, $3, true))); }
| TPLUS TSCAN expr
    { $$ = new CallExpr(new DefExpr(build_reduce(new SymExpr("_sum"), $3, true))); }
| TSTAR TSCAN expr
    { $$ = new CallExpr(new DefExpr(build_reduce(new SymExpr("_prod"), $3, true))); }
| TAND TSCAN expr
    { $$ = new CallExpr(new DefExpr(build_reduce(new SymExpr("_land"), $3, true))); }
| TOR TSCAN expr
    { $$ = new CallExpr(new DefExpr(build_reduce(new SymExpr("_lor"), $3, true))); }
| TBAND TSCAN expr
    { $$ = new CallExpr(new DefExpr(build_reduce(new SymExpr("_band"), $3, true))); }
| TBOR TSCAN expr
    { $$ = new CallExpr(new DefExpr(build_reduce(new SymExpr("_bor"), $3, true))); }
| TBXOR TSCAN expr
    { $$ = new CallExpr(new DefExpr(build_reduce(new SymExpr("_bxor"), $3, true))); }
;


/** TAGS *********************************************************************/

formal_tag:
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


is_config:
  /* nothing */
    { $$ = false; }
| TCONFIG
    { $$ = true; }
;


%%
