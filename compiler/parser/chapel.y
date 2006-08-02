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

#define YYLLOC_DEFAULT(Current, Rhs, N)          \
  if (N) { \
    (Current).first_line   = (Rhs)[1].first_line;      \
    if ((Current).first_line) yystartlineno = (Current).first_line; \
    (Current).first_column = (Rhs)[1].first_column;    \
    (Current).last_line    = (Rhs)[N].last_line;       \
    (Current).last_column  = (Rhs)[N].last_column; \
  } else (Current) = yylloc;

#ifdef __CYGWIN__
#define YYSTACK_ALLOC MALLOC
#endif

%}

%start program

%union  {
  bool boolval;
  char* pch;
  Vec<char*>* vpch;

  varType vt;
  consType ct;
  intentTag pt;
  BlockTag btag;
  fnType ft;

  Expr* pexpr;
  DefExpr* pdefexpr;

  Stmt* pstmt;
  WhenStmt* pwhenstmt;
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
%token TBEGIN
%token TBREAK
%token TCALL
%token TCLASS
%token TCOBEGIN
%token TCONFIG
%token TCONST
%token TCONSTRUCTOR
%token TPARAM
%token TCONTINUE
%token TDO
%token TDOMAIN
%token TENUM
%token TFOR
%token TFORALL
%token TFUN
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
%token TORDERED
%token TOTHERWISE
%token TOUT
%token TPRAGMA
%token TPRIMITIVE
%token TRECORD
%token TREF
%token TRETURN
%token TSELECT
%token TSPARSE
%token TSTATIC
%token TSYNC
%token TTHEN
%token TTYPE
%token TTYPEDEF
%token TUNION
%token TUSE
%token TVAL
%token TVAR
%token TWHEN
%token TWHERE
%token TWHILE
%token TYIELD

%token TIDENT
%token INTLITERAL FLOATLITERAL IMAGLITERAL UINTLITERAL
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
%token TDOTDOTDOT
%token TCOLON
%token TNOTCOLON
%token TQUESTION
%token TPARTIAL
%token TLP
%token TRP
%token TSEQBEGIN
%token TSEQEND
%token TLSBR
%token TRSBR
%token TLCBR
%token TRCBR

%type <vt> var_state_tag
%type <ct> var_const_tag
 
%type <pt> formal_tag
%type <ft> fn_tag
%type <btag> atomic_cobegin
%type <btag> for_tag

%type <boolval> fnretref

%type <pch> identifier fname opt_identifier
%type <pch> pragma
%type <vpch> pragma_ls

%type <pstmtls> program
%type <pstmtls> stmt_ls decl_stmt_ls

%type <pblockstmt> stmt empty_stmt label_stmt goto_stmt break_stmt continue_stmt
%type <pblockstmt> call_stmt expr_stmt if_stmt for_stmt while_do_stmt do_while_stmt
%type <pblockstmt> select_stmt return_stmt yield_stmt assign_stmt decl_stmt
%type <pblockstmt> type_select_stmt

%type <pblockstmt> typedef_decl_stmt fn_decl_stmt class_decl_stmt mod_decl_stmt
%type <pblockstmt> typevar_decl_stmt enum_decl_stmt use_stmt

%type <pblockstmt> var_decl_stmt
%type <pstmtls> var_decl_stmt_inner var_decl_stmt_inner_ls

%type <pblockstmt> parsed_block_single_stmt

%type <pblockstmt> parsed_block_stmt block_stmt

%type <pwhenstmt> when_stmt
%type <pwhenstmtls> when_stmt_ls

%type <pexpr> opt_var_type var_type fnrettype opt_formal_var_type formal_var_type formal_type
%type <pexpr> type record_tuple_type
%type <pexpr> record_tuple_inner_type
%type <pstmtls> record_inner_type_ls
%type <ptype> class_tag

%type <pexpr> parenop_expr memberaccess_expr non_tuple_lvalue lvalue
%type <pexpr> tuple_paren_expr expr expr_list_item opt_expr
%type <pexpr> literal range seq_expr where
%type <pexpr> variable_expr top_level_expr
%type <pexpr> reduction opt_init_expr var_arg_expr
%type <pexprls> expr_ls nonempty_expr_ls tuple_inner_type_ls opt_inherit_expr_ls
%type <pdefexpr> formal enum_item
%type <pdefexprls> formal_ls opt_formal_ls enum_ls

%type <pfnsym> function

/* These are declared in increasing order of precedence. */

%left TNOELSE
%left TELSE
%left TCOMMA
%left TREDUCE TSCAN
%left TIF
%left TBY
%left TRSBR
%left TIN
%left TDOTDOT
%left TSTARTUPLE
%right TPARTIAL
%left TSEQCAT
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
%right TUPLUS TUMINUS TBNOT
%right TEXP
%left TCOLON TNOTCOLON
%left TLP
%left TDOT

%% 


program: stmt_ls
    { (void)@1.first_line; yystmtlist = $$; }
;


/** STATEMENTS ***************************************************************/


stmt_ls:
  /* empty */
    { $$ = new AList<Stmt>(); }
| stmt_ls pragma_ls stmt
    {
      $3->body->first()->addPragmas($2);
      $1->insertAtTail($3);
    }
;


stmt:
  empty_stmt
| label_stmt
| goto_stmt
| break_stmt
| continue_stmt
| call_stmt
| expr_stmt
| if_stmt
| for_stmt
| while_do_stmt
| do_while_stmt
| select_stmt
| type_select_stmt
| return_stmt
| yield_stmt
| assign_stmt
| block_stmt
| decl_stmt
| error
    { printf("syntax error"); exit(1); }
;


parsed_block_single_stmt:
  empty_stmt
| label_stmt
| goto_stmt
| break_stmt
| continue_stmt
| call_stmt
| if_stmt
| select_stmt
| for_stmt
| while_do_stmt
| return_stmt
| yield_stmt
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
      $$->insertAtTail(buildLabelStmt(stringcat("_post", $2)));
      $$->insertAtHead(buildLabelStmt($2));
    }
;


goto_stmt:
  TGOTO identifier TSEMI
    { $$ = build_chpl_stmt(new GotoStmt(goto_normal, $2)); }
;


break_stmt:
  TBREAK opt_identifier TSEMI
    { $$ = build_chpl_stmt(new GotoStmt(goto_break, $2)); }
;


continue_stmt:
  TCONTINUE opt_identifier TSEMI
    { $$ = build_chpl_stmt(new GotoStmt(goto_continue, $2)); }
;


call_stmt:
  TCALL lvalue TSEMI
    { $$ = build_chpl_stmt($2); }
;


expr_stmt:
  top_level_expr TSEMI
    { $$ = build_chpl_stmt($1); }
;


if_stmt:
  TIF expr parsed_block_stmt            %prec TNOELSE
    { $$ = build_chpl_stmt(new CondStmt($2, $3)); }
| TIF expr TTHEN stmt                   %prec TNOELSE
    { $$ = build_chpl_stmt(new CondStmt($2, $4)); }
| TIF expr parsed_block_stmt TELSE stmt
    { $$ = build_chpl_stmt(new CondStmt($2, $3, $5)); }
| TIF expr TTHEN stmt TELSE stmt
    { $$ = build_chpl_stmt(new CondStmt($2, $4, $6)); }
;


for_stmt:
  TFOR TPARAM identifier TIN expr TDOTDOT expr TDO stmt
    { $$ = build_chpl_stmt(build_param_for($3, $5, $7, $9)); }
| TFOR TPARAM identifier TIN expr TDOTDOT expr parsed_block_stmt
    { $$ = build_chpl_stmt(build_param_for($3, $5, $7, $8)); }
| for_tag nonempty_expr_ls TIN nonempty_expr_ls parsed_block_stmt
    { $$ = build_chpl_stmt(build_for_block($1, exprsToIndices($2), $4, $5)); }
| for_tag nonempty_expr_ls TIN nonempty_expr_ls TDO stmt
    { $$ = build_chpl_stmt(build_for_block($1, exprsToIndices($2), $4, new BlockStmt($6))); }
| TLSBR nonempty_expr_ls TIN nonempty_expr_ls TRSBR stmt
    { $$ = build_chpl_stmt(build_for_block(BLOCK_FORALL, exprsToIndices($2), $4, new BlockStmt($6))); }
;


while_do_stmt:
  TWHILE expr TDO stmt
    { $$ = build_chpl_stmt(build_while_do_block($2, new BlockStmt($4))); }
| TWHILE expr parsed_block_stmt
    { $$ = build_chpl_stmt(build_while_do_block($2, $3)); }
;


do_while_stmt:
TDO stmt TWHILE expr TSEMI
    { $$ = build_chpl_stmt(build_do_while_block($4, $2)); }
;


type_select_stmt:
  TTYPE TSELECT nonempty_expr_ls TLCBR when_stmt_ls TRCBR
    { $$ = build_chpl_stmt(build_type_select($3, $5)); }
;


select_stmt:
  TSELECT expr TLCBR when_stmt_ls TRCBR
    { $$ = build_chpl_stmt(build_select($2, $4)); }
;


when_stmt_ls:
  /* empty */
    { $$ = new AList<WhenStmt>(); }
| when_stmt_ls when_stmt
    { $1->insertAtTail($2); }
; 


when_stmt:
  TWHEN nonempty_expr_ls TDO stmt
    { $$ = new WhenStmt($2, $4); }
| TWHEN nonempty_expr_ls parsed_block_stmt
    { $$ = new WhenStmt($2, $3); }
| TOTHERWISE stmt
    { $$ = new WhenStmt(new AList<Expr>(), $2); }
;


return_stmt:
  TRETURN opt_expr TSEMI
    { $$ = build_chpl_stmt(new ReturnStmt($2)); }
;


yield_stmt:
  TYIELD opt_expr TSEMI
    { $$ = build_chpl_stmt(new ReturnStmt($2, true)); }
;


assign_stmt:
  lvalue TASSIGN expr TSEMI
    {
      $$ = build_chpl_stmt(new CallExpr("=", $1, $3));
    }
| lvalue TASSIGNPLUS expr TSEMI
    {
      $$ = build_chpl_stmt(build_assignplus($1, $3));
    }
| lvalue TASSIGNMINUS expr TSEMI
    {
      $$ = build_chpl_stmt(new CallExpr("=", $1,
             new CallExpr("-", $1->copy(), $3)));
    }
| lvalue TASSIGNMULTIPLY expr TSEMI
    {
      $$ = build_chpl_stmt(new CallExpr("=", $1,
             new CallExpr("*", $1->copy(), $3)));
    }
| lvalue TASSIGNDIVIDE expr TSEMI
    {
      $$ = build_chpl_stmt(new CallExpr("=", $1,
             new CallExpr("/", $1->copy(), $3)));
    }
| lvalue TASSIGNBAND expr TSEMI
    {
      $$ = build_chpl_stmt(new CallExpr("=", $1,
             new CallExpr("&", $1->copy(), $3)));
    }
| lvalue TASSIGNBOR expr TSEMI
    {
      $$ = build_chpl_stmt(new CallExpr("=", $1,
             new CallExpr("|", $1->copy(), $3)));
    }
| lvalue TASSIGNBXOR expr TSEMI
    {
      $$ = build_chpl_stmt(new CallExpr("=", $1,
             new CallExpr("^", $1->copy(), $3)));
    }
| lvalue TASSIGNSEQCAT expr TSEMI
    {
      $$ = build_chpl_stmt(new CallExpr("=", $1,
             new CallExpr("#", $1->copy(), $3)));
    }
;


block_stmt:
  atomic_cobegin TLCBR stmt_ls TRCBR
    {
      $$ = build_chpl_stmt(new BlockStmt($3, $1));
    }
;


/** DECLARATION STATEMENTS ***************************************************/


decl_stmt_ls:
  /* empty */
    { $$ = new AList<Stmt>(); }
| decl_stmt_ls pragma_ls decl_stmt
    {
      $3->body->first()->addPragmas($2);
      $1->insertAtTail($3);
    }
;


decl_stmt:
  use_stmt
| mod_decl_stmt
| fn_decl_stmt
| class_decl_stmt
| enum_decl_stmt
| typedef_decl_stmt
| typevar_decl_stmt
| var_decl_stmt
;


use_stmt:
  TUSE lvalue TSEMI
    { $$ = build_chpl_stmt(new CallExpr(PRIMITIVE_USE, $2)); }
;


mod_decl_stmt:
  TMODULE identifier TLCBR stmt_ls TRCBR
    {
      $$ = build_chpl_stmt(new ExprStmt(new DefExpr(build_module($2, MOD_USER, $4))));
    }
;


fn_decl_stmt:
  fn_tag function opt_formal_ls fnretref fnrettype where parsed_block_stmt
    {
      $2->fnClass = $1;
      if ($1 == FN_ITERATOR && !strcmp($2->name, "this"))
        $2->name = "_promoter";
      if (!$3)
        $2->noParens = true;
      else
        $2->formals->insertAtTail($3);
      $2->retRef = $4;
      $2->retExpr = $5;
      if ($6)
        $2->where = new BlockStmt(new ExprStmt($6));
      $2->body = new BlockStmt($7);
      $$ = build_chpl_stmt(new DefExpr($2));
    }
;


fn_tag:
  TFUN
    { $$ = FN_FUNCTION; }
| TCONSTRUCTOR
    { $$ = FN_CONSTRUCTOR; }
| TITERATOR
    { $$ = FN_ITERATOR; }
;


opt_formal_ls:
  { $$ = NULL; }
| TLP formal_ls TRP
  { $$ = $2; }
;


formal_ls:
  /* empty */
    { $$ = new AList<DefExpr>(); }
| formal
    { $$ = new AList<DefExpr>($1); }
| formal_ls TCOMMA formal
    { $1->insertAtTail($3); }
;


var_arg_expr:
  TDOTDOTDOT expr
    { $$ = $2; }
| TDOTDOTDOT TQUESTION identifier
    { $$ = new DefExpr(new VarSymbol($3, dtUnknown, VAR_NORMAL, VAR_PARAM)); }
;


formal:
  formal_tag pragma_ls identifier opt_formal_var_type opt_init_expr
    {
      $$ = build_arg($1, $3, $4, $5, NULL);
      $$->sym->addPragmas($2);
    }
| formal_tag pragma_ls identifier opt_formal_var_type var_arg_expr
    {
      $$ = build_arg($1, $3, $4, NULL, $5);
      $$->sym->addPragmas($2);
    }
| TLP formal_ls TRP
    {
      ClassType *t = new ClassType(CLASS_RECORD);
      // NOTE:
      //   change DefExpr into ExprStmt because functions take 
      //   a list of DefExpr but records take a list of Stmt
      // NOTE:
      //   this record has members which are ArgSymbols
      AList<Stmt> *stmts = new AList<Stmt>;
      for_alist(DefExpr, x, $2) {
        stmts->insertAtTail(x);
      }
      build_class(NULL, t, stmts);
      $$ = build_arg(INTENT_IN, "<anonymous>", NULL, NULL, NULL);
      $$->sym->type = t;
      t->isPattern = true;
    }
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
| TAND
  { $$ = "&&"; } 
| TNOT
  { $$ = "!"; }
| TOR
  { $$ = "||"; } 
| TSEQCAT
  { $$ = "#"; } 
| TBY
  { $$ = "by"; } 
  ;

where:
  /* empty */
    { $$ = NULL; }
| TWHERE expr
    { $$ = $2; }
;

function:
  fname
    { $$ = new FnSymbol($1); }
| TASSIGN identifier
    {
      $$ = new FnSymbol($2);
      $$->isSetter = true;
    }
| identifier TDOT fname
    { $$ = new FnSymbol($3, new UnresolvedSymbol($1)); }
| identifier TDOT TASSIGN identifier
    {
      $$ = new FnSymbol($4, new UnresolvedSymbol($1));
      $$->isSetter = true;
    }
| TLP non_tuple_lvalue TRP TDOT identifier
    {
      $$ = new FnSymbol($5);
      $$->_this = new ArgSymbol(INTENT_BLANK, "this", dtUnknown);
      $$->formals->insertAtHead(new DefExpr($$->_this, NULL, $2));
      $$->formals->insertAtHead(new DefExpr(new ArgSymbol(INTENT_BLANK, "_methodTokenDummy",
                                                          dtMethodToken)));
    }
| TLP non_tuple_lvalue TRP TDOT TASSIGN identifier
    {
      $$ = new FnSymbol($6);
      $$->_this = new ArgSymbol(INTENT_BLANK, "this", dtUnknown);
      $$->formals->insertAtHead(new DefExpr($$->_this, NULL, $2));
      $$->formals->insertAtHead(new DefExpr(new ArgSymbol(INTENT_BLANK, "_methodTokenDummy",
                                                          dtMethodToken)));
      $$->formals->last()->insertBefore(new DefExpr(new ArgSymbol(INTENT_BLANK, "_setterTokenDummy", dtSetterToken)));
    }
;


class_decl_stmt:
  class_tag pragma_ls identifier opt_inherit_expr_ls TLCBR decl_stmt_ls TRCBR
    {
      DefExpr* def = build_class($3, $1, $6);
      def->sym->addPragmas($2);
      dynamic_cast<ClassType*>(dynamic_cast<TypeSymbol*>(def->sym)->type)->inherits = $4;
      $$ = build_chpl_stmt(def);
    }
;


class_tag:
  TCLASS
    { $$ = new ClassType(CLASS_CLASS); }
| TRECORD
    { $$ = new ClassType(CLASS_RECORD); }
| TUNION
    { USR_FATAL("Unions are not yet supported"); }
;


opt_inherit_expr_ls:
  /* empty */
    { $$ = new AList<Expr>(); }
| TCOLON nonempty_expr_ls
    { $$ = $2; }
;


enum_decl_stmt:
  TENUM pragma_ls identifier TLCBR enum_ls TRCBR TSEMI
    {
      EnumType* pdt = new EnumType($5);
      TypeSymbol* pst = new TypeSymbol($3, pdt);
      pst->addPragmas($2);
      DefExpr* def_expr = new DefExpr(pst);
      $$ = build_chpl_stmt(def_expr);
    }
;


enum_ls:
  enum_item
    { $$ = new AList<DefExpr>($1); }
| enum_ls TCOMMA enum_item
    { $1->insertAtTail($3); }
;


enum_item:
  identifier
    { $$ = new DefExpr(new EnumSymbol($1)); }
| identifier TASSIGN expr
    { $$ = new DefExpr(new EnumSymbol($1), $3); }
;


typedef_decl_stmt:
  TTYPEDEF pragma_ls identifier TCOLON type TSEMI
    {
      UserType* newtype = new UserType($5);
      TypeSymbol* typeSym = new TypeSymbol($3, newtype);
      typeSym->addPragmas($2);
      DefExpr* def_expr = new DefExpr(typeSym);
      $$ = build_chpl_stmt(def_expr);
    }
;


typevar_decl_stmt:
  TTYPE pragma_ls identifier TSEMI
    {
      VarSymbol* var = new VarSymbol($3);
      var->isTypeVariable = true;
      var->addPragmas($2);
      DefExpr* def = new DefExpr(var);
      $$ = build_chpl_stmt(def);
    }
;


var_decl_stmt:
  var_state_tag var_const_tag var_decl_stmt_inner_ls TSEMI
    {
      setVarSymbolAttributes($3, $1, $2);
      $$ = build_chpl_stmt($3);
    }
;


var_state_tag:
  /* nothing */
    { $$ = VAR_NORMAL; }
| TCONFIG
    { $$ = VAR_CONFIG; }
| TSTATIC
    { $$ = VAR_STATE; }
;


var_const_tag:
  TVAR
    { $$ = VAR_VAR; }
| TCONST
    { $$ = VAR_CONST; }
| TPARAM
    { $$ = VAR_PARAM; }
;


var_decl_stmt_inner_ls:
  var_decl_stmt_inner
| var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner
    {
      $1->insertAtTail($3);
    }
;


var_decl_stmt_inner:
  identifier opt_var_type opt_init_expr
    {
      VarSymbol* var = new VarSymbol($1);
      $$ = new AList<Stmt>(new DefExpr(var, $3, $2));
    }
;


/** TYPES ********************************************************************/


record_tuple_inner_type:
  record_inner_type_ls TRP
    {
      $$ = build_class(stringcat("_anon_record", intstring(anon_record_uid++)), new ClassType(CLASS_RECORD), $1);
    }
| tuple_inner_type_ls TRP
    {
      CallExpr* call = new CallExpr("_tuple", new_IntLiteral($1->length()));
      for_alist(Expr, expr, $1) {
        call->argList->insertAtTail(new CallExpr("_init", expr->remove()));
      }
      $$ = call;
    }
;


record_tuple_type:
  TRECORD TLCBR decl_stmt_ls TRCBR
    {
      $$ = build_class(stringcat("_anon_record", intstring(anon_record_uid++)), new ClassType(CLASS_RECORD), $3);
    }
| TLP record_tuple_inner_type
    {
      $$ = $2;
    }
;


var_type:
  TCOLON type
    { $$ = $2; }
;


fnrettype:
  /* empty */
    { $$ = NULL; }
| TCOLON type
    { $$ = $2; }
;


opt_var_type:
  /* nothing */
    { $$ = NULL; }
| var_type
;


formal_var_type:
  TCOLON formal_type
    { $$ = $2; }
;


opt_formal_var_type:
  /* nothing */
    { $$ = NULL; }
| formal_var_type
;


opt_init_expr:
  /* nothing */
    { $$ = NULL; }
| TASSIGN expr
    { $$ = $2; }
;


tuple_inner_type_ls:
  type
    { $$ = new AList<Expr>($1); }
| tuple_inner_type_ls TCOMMA type
    { $1->insertAtTail($3); }
;


record_inner_type_ls:
  identifier var_type opt_init_expr
    { $$ = new AList<Stmt>(new DefExpr(new VarSymbol($1), $3, $2)); }
| record_inner_type_ls TCOMMA identifier var_type
    { $1->insertAtTail(new DefExpr(new VarSymbol($3), NULL, $4)); }
;


variable_expr:
  identifier
    { $$ = new SymExpr(new UnresolvedSymbol($1)); }
;


type:
  non_tuple_lvalue %prec TSTARTUPLE
| record_tuple_type
| non_tuple_lvalue TOF type
    { $$ = new CallExpr($1, new NamedExpr("elt_type", $3)); }
| non_tuple_lvalue TSTAR type %prec TSTAR
    { $$ = new CallExpr("_tuple", $1, new CallExpr("_init", $3)); }
| TLSBR nonempty_expr_ls TRSBR type
    { $$ = new CallExpr("_build_array_type", new CallExpr("_build_domain", $2), $4); }
| TDOMAIN TLP expr_ls TRP
    { $$ = new CallExpr("_build_domain_type", $3); }
| TDOMAIN
    { $$ = new SymExpr("_domain"); }
| TSPARSE TDOMAIN TLP expr_ls TRP
    { $$ = new CallExpr("_build_sparse_domain_type", $4); }
| TINDEX TLP expr_ls TRP
    { $$ = new CallExpr("_build_index_type", $3); }
| TSYNC type
    { $$ = new CallExpr( "_syncvar", $2); }
;

formal_type:
  type
| TQUESTION identifier
    {
      TypeSymbol* new_symbol = new TypeSymbol($2, new UserType(new SymExpr(gNil))); // gNil is a place holder to be fixed in cleanup
      $$ = new DefExpr(new_symbol, NULL, NULL);
    }
;

pragma_ls:
    { $$ = new Vec<char*>(); }
| pragma_ls pragma
    { $1->add($2); }
;


pragma:
  TPRAGMA STRINGLITERAL
{ $$ = stringcpy($2); }
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
;


nonempty_expr_ls:
  pragma_ls expr_list_item
    { $2->addPragmas($1); $$ = new AList<Expr>($2); }
| nonempty_expr_ls TCOMMA pragma_ls expr_list_item
    { $4->addPragmas($3); $1->insertAtTail($4); }
;


tuple_paren_expr:
  TLP nonempty_expr_ls TRP 
    { 
      if ($2->length() == 1) {
        $$ = $2->get(1);
        $$->remove();
      } else {
        $$ = new CallExpr("_tuple", new_IntLiteral($2->length()), $2);
      }
    }
;


parenop_expr:
  non_tuple_lvalue TLP expr_ls TRP
    { $$ = new CallExpr($1, $3); }
| TPRIMITIVE TLP expr_ls TRP
    {
      $$ = build_primitive_call($3);
    }
;


memberaccess_expr:
  non_tuple_lvalue TDOT identifier
    { $$ = new CallExpr(".", $1, new_StringSymbol($3)); }
| non_tuple_lvalue TDOT TTYPE
    { $$ = new CallExpr(PRIMITIVE_TYPEOF, $1); }
;


non_tuple_lvalue:
  literal
| variable_expr
| parenop_expr
| memberaccess_expr
;


lvalue:
  non_tuple_lvalue
| tuple_paren_expr
;


seq_expr:
  TSEQBEGIN expr_ls TSEQEND
    {
      if ($2->length() == 0)
        USR_FATAL($2, "Invalid empty sequence literal '(//)'");
      Expr* seqLiteral = new CallExpr("seq", $2->first()->copy());
      for_alist(Expr, element, $2) {
        element->remove();
        seqLiteral =
          new CallExpr(
            new CallExpr(".", seqLiteral, new_StringSymbol("_append_in_place")),
            element);
      }
      $$ = seqLiteral;
    }
;


opt_expr:
  /* empty */
    { $$ = NULL; }
| expr
;


expr:
  top_level_expr
| TLSBR nonempty_expr_ls TIN nonempty_expr_ls TRSBR expr %prec TRSBR
    {
      FnSymbol* forall_iterator =
        new FnSymbol(stringcat("_forallexpr", intstring(iterator_uid++)));
      forall_iterator->insertAtTail(build_for_expr(exprsToIndices($2), $4, $6));
      $$ = new CallExpr(new DefExpr(forall_iterator));
    }
| TLSBR nonempty_expr_ls TRSBR
    { $$ = new CallExpr("_build_domain", $2); }
| TIF expr TTHEN expr TELSE expr
    { $$ = new CallExpr(new DefExpr(build_if_expr($2, $4, $6))); }
| TIF expr TTHEN expr %prec TNOELSE
    { $$ = new CallExpr(new DefExpr(build_if_expr($2, $4))); }
;


top_level_expr: 
  lvalue
| TLP TDOTDOTDOT expr TRP
    { $$ = new CallExpr(PRIMITIVE_TUPLE_EXPAND, $3); }
| TNIL
    { $$ = new SymExpr(gNil); }
| TLET var_decl_stmt_inner_ls TIN expr
    { $$ = new CallExpr(new DefExpr(build_let_expr($2, $4))); }
| reduction %prec TREDUCE
| TPARTIAL expr
    {
      $$ = $2;
      if (CallExpr* call = dynamic_cast<CallExpr*>($$)) {
        call->partialTag = PARTIAL_ALWAYS;
      } else {
        USR_FATAL($$, "Unable to partially evaluate non-call");
      }
    }
| expr TCOLON type
    {
      $$ = new CallExpr(PRIMITIVE_CAST, $3, $1);
    }
/* | expr TCOLON STRINGLITERAL */
/*   {  */
/*     $$ = new CallExpr("_tostring", $1, new_StringLiteral($3)); */
/*   } */
| range %prec TDOTDOT
| seq_expr
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
    { $$ = new CallExpr("&&", $1, $3); }
| expr TOR expr
    { $$ = new CallExpr("||", $1, $3); }
| expr TEXP expr
    { $$ = new CallExpr("**", $1, $3); }
| expr TSEQCAT expr
    { $$ = new CallExpr("#", $1, $3); }
| expr TBY expr
    { $$ = new CallExpr("by", $1, $3); }
;

reduction:
  expr TREDUCE expr
    { $$ = build_reduce_expr($1, $3); }
| identifier TSCAN expr
    { $$ = new CallExpr(new UnresolvedSymbol("_scan"), new UnresolvedSymbol($1), $3); }
;


range:
  expr TDOTDOT expr
    { $$ = new CallExpr("_aseq", $1, $3, new_IntLiteral(1)); }
;


literal:
  INTLITERAL
    { $$ = new_IntLiteral(yytext); }
| UINTLITERAL
    { $$ = new_UIntLiteral(yytext); }
| FLOATLITERAL
    { $$ = new_FloatLiteral(yytext, atof(yytext)); }
| IMAGLITERAL
    {
      yytext[strlen(yytext)-1] = '\0';
      $$ = new_ComplexLiteral(atof(yytext));
    }
| STRINGLITERAL
    { $$ = new_StringLiteral($1); }
;


identifier:
  TIDENT
    { $$ = stringcpy(yytext); }
;


opt_identifier:
  /* empty */
    { $$ = NULL; }
| identifier
;


/** TAGS *********************************************************************/


atomic_cobegin:
    { $$ = BLOCK_NORMAL; }
| TATOMIC
    { $$ = BLOCK_ATOMIC; }
| TBEGIN
    { $$ = BLOCK_BEGIN; }
| TCOBEGIN
    { $$ = BLOCK_COBEGIN; }
;


for_tag:
  TFOR
    { $$ = BLOCK_FOR; }
| TFORALL
    { $$ = BLOCK_FORALL; }
| TORDERED TFORALL
    { $$ = BLOCK_ORDERED_FORALL; }
;


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


%%
