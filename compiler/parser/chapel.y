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

%}

%start program

%union  {
  bool boolval;
  char* pch;
  Vec<char*>* vpch;

  varType vt;
  consType ct;
  intentTag pt;
  blockStmtType blktype;
  fnType ft;
  ForLoopStmtTag flstag;

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
%token TTYPEDEF
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
%token INTLITERAL FLOATLITERAL IMAGLITERAL
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

%type <vt> var_state_tag
%type <ct> var_const_tag
 
%type <pt> formal_tag
%type <ft> fn_tag
%type <blktype> atomic_cobegin
%type <flstag> for_stmt_tag

%type <boolval> fnretref

%type <pch> identifier fname opt_identifier
%type <pch> pragma
%type <vpch> pragma_ls

%type <pstmtls> program
%type <pstmtls> stmt_ls decl_stmt_ls

%type <pstmtls> stmt empty_stmt label_stmt goto_stmt break_stmt continue_stmt
%type <pstmtls> call_stmt expr_stmt if_stmt for_stmt while_do_stmt do_while_stmt
%type <pstmtls> select_stmt return_stmt yield_stmt assign_stmt decl_stmt

%type <pstmtls> typedef_decl_stmt fn_decl_stmt class_decl_stmt mod_decl_stmt
%type <pstmtls> typevar_decl_stmt enum_decl_stmt with_stmt use_stmt where_stmt

%type <pstmtls> var_decl_stmt var_decl_stmt_inner var_decl_stmt_inner_ls


%type <pstmtls> parsed_block_single_stmt
%type <pblockstmt> parsed_block_stmt block_stmt

%type <pwhenstmt> when_stmt
%type <pwhenstmtls> when_stmt_ls

%type <pexpr> opt_var_type var_type fnrettype opt_formal_var_type formal_var_type formal_type
%type <pexpr> type record_tuple_type
%type <pexpr> record_tuple_inner_type
%type <pstmtls> record_inner_type_ls
%type <ptype> class_tag

%type <pexpr> parenop_expr memberaccess_expr non_tuple_lvalue lvalue
%type <pexpr> tuple_paren_expr atom expr expr_list_item opt_expr
%type <pexpr> literal range seq_expr where whereexpr
%type <pexpr> tuple_multiplier variable_expr top_level_expr
%type <pexpr> reduction opt_init_expr
%type <pexprls> expr_ls nonempty_expr_ls tuple_inner_type_ls opt_inherit_expr_ls
%type <pdefexpr> formal enum_item
%type <pdefexprls> formal_ls opt_formal_ls enum_ls

%type <pfnsym> function

/* These are declared in increasing order of precedence. */

%left TNOELSE
%left TELSE
%left TCOMMA
%left TIF
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


program: stmt_ls
    { yystmtlist = $$; }
;


/** STATEMENTS ***************************************************************/


stmt_ls:
  /* empty */
    { $$ = new AList<Stmt>(); }
| stmt_ls pragma_ls stmt
    { 
      $3->first()->addPragmas($2);
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
| return_stmt
| yield_stmt
| assign_stmt
| block_stmt
    { $$ = new AList<Stmt>($1); }
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
    { $$ = new AList<Stmt>(new BlockStmt()); }
;


label_stmt:
  TLABEL identifier stmt
    {
      $3->insertAtHead(new LabelStmt($2));
      $$ = $3;
    }
;


goto_stmt:
  TGOTO identifier TSEMI
    { $$ = new AList<Stmt>(new GotoStmt(goto_normal, $2)); }
;


break_stmt:
  TBREAK opt_identifier TSEMI
    { $$ = new AList<Stmt>(new GotoStmt(goto_break, $2)); }
;


continue_stmt:
  TCONTINUE opt_identifier TSEMI
    { $$ = new AList<Stmt>(new GotoStmt(goto_continue, $2)); }
;


call_stmt:
  TCALL lvalue TSEMI
    { $$ = new AList<Stmt>(new ExprStmt($2)); }
;


expr_stmt:
  top_level_expr TSEMI
    { $$ = new AList<Stmt>(new ExprStmt($1)); }
;


if_stmt:
  TIF expr parsed_block_stmt            %prec TNOELSE
    { $$ = new AList<Stmt>(new CondStmt($2, $3)); }
| TIF expr TTHEN stmt                   %prec TNOELSE
    { $$ = new AList<Stmt>(new CondStmt($2, $4)); }
| TIF expr parsed_block_stmt TELSE stmt
    { $$ = new AList<Stmt>(new CondStmt($2, $3, $5)); }
| TIF expr TTHEN stmt TELSE stmt
    { $$ = new AList<Stmt>(new CondStmt($2, $4, $6)); }
;


for_stmt:
  for_stmt_tag nonempty_expr_ls TIN nonempty_expr_ls parsed_block_stmt
    { $$ = new AList<Stmt>(new ForLoopStmt($1, exprsToIndices($2), $4, $5)); }
| for_stmt_tag nonempty_expr_ls TIN nonempty_expr_ls TDO stmt
    { $$ = new AList<Stmt>(new ForLoopStmt($1, exprsToIndices($2), $4, $6)); }
| TLSBR nonempty_expr_ls TIN nonempty_expr_ls TRSBR stmt
    {
      $$ = new AList<Stmt>(
             new ForLoopStmt(FORLOOPSTMT_FORALL, exprsToIndices($2), $4, $6));
    }
;


while_do_stmt:
TWHILE expr TDO stmt
    { $$ = new AList<Stmt>(new WhileLoopStmt(true, $2, $4)); }
| TWHILE expr parsed_block_stmt
    { $$ = new AList<Stmt>(new WhileLoopStmt(true, $2, $3)); }
;


do_while_stmt:
TDO stmt TWHILE expr TSEMI
    { $$ = new AList<Stmt>(new WhileLoopStmt(false, $4, $2)); }
;


select_stmt:
  TSELECT expr TLCBR when_stmt_ls TRCBR
    { $$ = new AList<Stmt>(new SelectStmt($2, $4)); }
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
    { $$ = new AList<Stmt>(new ReturnStmt($2)); }
;


yield_stmt:
  TYIELD opt_expr TSEMI
    { $$ = new AList<Stmt>(new ReturnStmt($2, true)); }
;


assign_stmt:
  lvalue TASSIGN expr TSEMI
    {
      $$ = new AList<Stmt>(new ExprStmt(new CallExpr("=", $1, $3)));
    }
| lvalue TASSIGNPLUS expr TSEMI
    {
      $$ = new AList<Stmt>(new ExprStmt(new CallExpr("=", $1,
             new CallExpr("+", $1->copy(), $3))));
    }
| lvalue TASSIGNMINUS expr TSEMI
    {
      $$ = new AList<Stmt>(new ExprStmt(new CallExpr("=", $1,
             new CallExpr("-", $1->copy(), $3))));
    }
| lvalue TASSIGNMULTIPLY expr TSEMI
    {
      $$ = new AList<Stmt>(new ExprStmt(new CallExpr("=", $1,
             new CallExpr("*", $1->copy(), $3))));
    }
| lvalue TASSIGNDIVIDE expr TSEMI
    {
      $$ = new AList<Stmt>(new ExprStmt(new CallExpr("=", $1,
             new CallExpr("/", $1->copy(), $3))));
    }
| lvalue TASSIGNBAND expr TSEMI
    {
      $$ = new AList<Stmt>(new ExprStmt(new CallExpr("=", $1,
             new CallExpr("&", $1->copy(), $3))));
    }
| lvalue TASSIGNBOR expr TSEMI
    {
      $$ = new AList<Stmt>(new ExprStmt(new CallExpr("=", $1,
             new CallExpr("|", $1->copy(), $3))));
    }
| lvalue TASSIGNBXOR expr TSEMI
    {
      $$ = new AList<Stmt>(new ExprStmt(new CallExpr("=", $1,
             new CallExpr("^", $1->copy(), $3))));
    }
| lvalue TASSIGNSEQCAT expr TSEMI
    {
      $$ = new AList<Stmt>(new ExprStmt(new CallExpr("=", $1,
             new CallExpr("#", $1->copy(), $3))));
    }
;


block_stmt:
  atomic_cobegin TLCBR stmt_ls TRCBR
    {
      $$ = new BlockStmt($3, $1);
    }
;


/** DECLARATION STATEMENTS ***************************************************/


decl_stmt_ls:
  /* empty */
    { $$ = new AList<Stmt>(); }
| decl_stmt_ls pragma_ls decl_stmt
    {
      $3->first()->addPragmas($2);
      $1->insertAtTail($3);
    }
;


decl_stmt:
  with_stmt
| use_stmt
| where_stmt
| mod_decl_stmt
| fn_decl_stmt
| class_decl_stmt
| enum_decl_stmt
| typedef_decl_stmt
| typevar_decl_stmt
| var_decl_stmt
;


with_stmt:
  TWITH lvalue TSEMI
    { $$ = new AList<Stmt>(new ExprStmt(new ImportExpr(IMPORT_WITH, $2))); }
;


use_stmt:
  TUSE lvalue TSEMI
    { $$ = new AList<Stmt>(new ExprStmt(new ImportExpr(IMPORT_USE, $2))); }
;


where_stmt:
  TWHERE whereexpr TSEMI
    { $$ = new AList<Stmt>(new ExprStmt($2)); }
;


mod_decl_stmt:
  TMODULE identifier
    {
      $<pmodsym>$ = Symboltable::startModuleDef($2);
    }
                     TLCBR stmt_ls TRCBR
    {
      $$ = new AList<Stmt>(new ExprStmt(Symboltable::finishModuleDef($<pmodsym>3, $5)));
    }
;


fn_decl_stmt:
  fn_tag function opt_formal_ls fnretref fnrettype where parsed_block_stmt
    {
      $2->fnClass = $1;
      if (!$3) {
        $3 = new AList<DefExpr>();
        $2->noParens = true;
      }
      $2->formals = $3;
      $2->retRef = $4;
      $2->whereExpr = $6;
      $2->body = $7;
      $$ = new AList<Stmt>(new ExprStmt(new DefExpr($2, NULL, $5)));
    }
;


fn_tag:
  TFUNCTION
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


formal:
  formal_tag pragma_ls identifier opt_formal_var_type opt_init_expr
    {
      $$ = Symboltable::defineParam($1, $3, $4, $5);
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
        stmts->insertAtTail(new ExprStmt(x));
      }
      Symboltable::defineStructType(NULL, t, stmts);
      $$ = Symboltable::defineParam(INTENT_IN, "<anonymous>", NULL, NULL);
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

where:
  /* empty */
    { $$ = NULL; }
| TWHERE whereexpr
    { $$ = $2; }
;

whereexpr: 
  atom
| TTYPE identifier opt_var_type TCOMMA whereexpr
    { 
      VariableType* new_type = new VariableType(getMetaType(0));
      TypeSymbol* new_symbol = new TypeSymbol($2, new_type);
      $$ = new DefExpr(new_symbol, NULL, $3);
    }
| TNOT whereexpr
    { $$ = new CallExpr("not", $2); }
| TBNOT whereexpr
    { $$ = new CallExpr("~", $2); }
| whereexpr TPLUS whereexpr
    { $$ = new CallExpr("+", $1, $3); }
| whereexpr TMINUS whereexpr
    { $$ = new CallExpr("-", $1, $3); }
| whereexpr TSTAR whereexpr
    { $$ = new CallExpr("*", $1, $3); }
| whereexpr TDIVIDE whereexpr
    { $$ = new CallExpr("/", $1, $3); }
| whereexpr TMOD whereexpr
    { $$ = new CallExpr("mod", $1, $3); }
| whereexpr TEQUAL whereexpr
    { $$ = new CallExpr("==", $1, $3); }
| whereexpr TNOTEQUAL whereexpr
    { $$ = new CallExpr("!=", $1, $3); }
| whereexpr TLESSEQUAL whereexpr
    { $$ = new CallExpr("<=", $1, $3); }
| whereexpr TGREATEREQUAL whereexpr
    { $$ = new CallExpr(">=", $1, $3); }
| whereexpr TLESS whereexpr
    { $$ = new CallExpr("<", $1, $3); }
| whereexpr TGREATER whereexpr
    { $$ = new CallExpr(">", $1, $3); }
| whereexpr TBAND whereexpr
    { $$ = new CallExpr("&", $1, $3); }
| whereexpr TBOR whereexpr
    { $$ = new CallExpr("|", $1, $3); }
| whereexpr TBXOR whereexpr
    { $$ = new CallExpr("~", $1, $3); }
| whereexpr TCOMMA whereexpr
    { $$ = new CallExpr("and", $1, $3); }
| whereexpr TOR whereexpr
    { $$ = new CallExpr("or", $1, $3); }
| whereexpr TEXP whereexpr
    { $$ = new CallExpr("**", $1, $3); }
| whereexpr TSEQCAT whereexpr
    { $$ = new CallExpr("#", $1, $3); }
| whereexpr TBY whereexpr
    { $$ = new CallExpr("by", $1, $3); }
| whereexpr TCOLON whereexpr
    { $$ = new CallExpr(OP_SUBTYPE, $1, $3); }
| whereexpr TNOTCOLON whereexpr
    { $$ = new CallExpr(OP_NOTSUBTYPE, $1, $3); }
| class_tag pragma_ls opt_identifier TLCBR decl_stmt_ls TRCBR
    { $$ = NULL; }
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
    { $$ = new FnSymbol($3, new TypeSymbol($1, NULL)); }
| identifier TDOT TASSIGN identifier
    {
      $$ = new FnSymbol($4, new TypeSymbol($1, NULL));
      $$->isSetter = true;
    }
;


class_decl_stmt:
  class_tag pragma_ls identifier opt_inherit_expr_ls TLCBR decl_stmt_ls TRCBR
    {
      DefExpr* def = Symboltable::defineStructType($3, $1, $6);
      def->sym->addPragmas($2);
      dynamic_cast<ClassType*>(dynamic_cast<TypeSymbol*>(def->sym)->definition)->inherits = $4;
      $$ = new AList<Stmt>(new ExprStmt(def));
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
      $$ = new AList<Stmt>(new ExprStmt(def_expr));
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
  TTYPEDEF pragma_ls identifier TCOLON type opt_init_expr TSEMI
    {
      UserType* newtype = new UserType($5, $6);
      TypeSymbol* typeSym = new TypeSymbol($3, newtype);
      typeSym->addPragmas($2);
      DefExpr* def_expr = new DefExpr(typeSym);
      $$ = new AList<Stmt>(new ExprStmt(def_expr));
    }
;


typevar_decl_stmt:
  TTYPE pragma_ls identifier opt_var_type TSEMI
    {
      VariableType* new_type = new VariableType(getMetaType(0));
      TypeSymbol* new_symbol = new TypeSymbol($3, new_type);
      new_symbol->addPragmas($2);
      DefExpr* def_expr = new DefExpr(new_symbol, NULL, $4);
      $$ = new AList<Stmt>(new ExprStmt(def_expr));
    }
;


var_decl_stmt:
  var_state_tag var_const_tag var_decl_stmt_inner_ls TSEMI
    {
      setVarSymbolAttributes($3, $1, $2);
      $$ = $3;
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
      $$ = new AList<Stmt>(new ExprStmt(new DefExpr(new VarSymbol($1), $3, $2)));
    }
;


/** TYPES ********************************************************************/


record_tuple_inner_type:
  record_inner_type_ls TRP
    {
      $$ = Symboltable::defineStructType(stringcat("_anon_record", intstring(anon_record_uid++)), new ClassType(CLASS_RECORD), $1);
    }
| tuple_inner_type_ls TRP
    {
      char *tupleName = stringcat("_tuple", intstring($1->length()));
      $$ = new CallExpr(tupleName, $1);
    }
;


record_tuple_type:
  TRECORD TLCBR decl_stmt_ls TRCBR
    {
      $$ = Symboltable::defineStructType(stringcat("_anon_record", intstring(anon_record_uid++)), new ClassType(CLASS_RECORD), $3);
    }
| TLP record_tuple_inner_type
    {
      $$ = $2;
    }
;


var_type:
  TCOLON type
    { $$ = $2; }
| TLIKE expr
    { $$ = new CallExpr("typeof", $2); }
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
| TLIKE expr
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
  lvalue
    { $$ = new AList<Expr>($1); }
| tuple_inner_type_ls TCOMMA lvalue
    { $1->insertAtTail($3); }
;


record_inner_type_ls:
  identifier var_type opt_init_expr
    { $$ = new AList<Stmt>(new ExprStmt(new DefExpr(new VarSymbol($1), $3, $2))); }
| record_inner_type_ls TCOMMA identifier var_type
    { $1->insertAtTail(new ExprStmt(new DefExpr(new VarSymbol($3), NULL, $4))); }
;


tuple_multiplier:
  INTLITERAL
    { $$ = new_IntLiteral(yytext); }
/* | non_tuple_lvalue */
;


variable_expr:
  identifier
    { $$ = new SymExpr(new UnresolvedSymbol($1)); }
;


type:
  variable_expr
| memberaccess_expr
| parenop_expr
| record_tuple_type
| non_tuple_lvalue TOF type
    { $$ = new CallExpr($1, new NamedExpr("elt_type", $3)); }
| tuple_multiplier TSTAR variable_expr
    { $$ = new CallExpr("_htuple", $3, $1); }
| TLSBR nonempty_expr_ls TRSBR type
    { $$ = new CallExpr("_aarray", $4, $2); }
;

formal_type:
  type
| TQUESTION identifier opt_var_type
    { 
      VariableType* new_type = new VariableType(getMetaType(0));
      TypeSymbol* new_symbol = new TypeSymbol($2, new_type);
      $$ = new DefExpr(new_symbol, NULL, $3);
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
        AList<Expr>* types = new AList<Expr>();
        AList<Expr>* fields = new AList<Expr>();
        char* tuple_name = stringcat("_tuple", intstring($2->length()));
        for_alist(Expr, expr, $2) {
          types->insertAtTail(new CallExpr("typeof", expr->copy()));
          fields->insertAtTail(expr->copy());
        }
        $$ = new CallExpr(tuple_name, types, fields);
      }
    }
;


parenop_expr:
  non_tuple_lvalue TLP expr_ls TRP
    { $$ = new CallExpr($1, $3); }
;


memberaccess_expr:
  non_tuple_lvalue TDOT identifier
    { $$ = new MemberAccess($1, $3); }
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
      Expr* seqType = new CallExpr("typeof", $2->first()->copy());
      Expr* seqLiteral = new CallExpr("seq", seqType);
      for_alist(Expr, element, $2) {
        element->remove();
        seqLiteral =
          new CallExpr(
            new MemberAccess(seqLiteral, "_append_in_place"),
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
      forall_iterator->fnClass = FN_ITERATOR;
      forall_iterator->body =
        new BlockStmt(
          new ForLoopStmt(FORLOOPSTMT_FORALL,
                          exprsToIndices($2),
                          $4,
                          new ReturnStmt($6, true)));
      $$ = new CallExpr(new DefExpr(forall_iterator));
    }
| TLSBR nonempty_expr_ls TRSBR
    { $$ = new CallExpr("_adomain_lit", $2); }
| TIF expr TTHEN expr TELSE expr
    { $$ = new CondExpr($2, $4, $6); }
| TIF expr TTHEN expr %prec TNOELSE
    { $$ = new CondExpr($2, $4); }
;


top_level_expr: 
  atom
| TNIL
    { $$ = new SymExpr(gNil); }
| TUNSPECIFIED
    { $$ = new SymExpr(gUnspecified); }
| TLET var_decl_stmt_inner_ls TIN expr
    {
      AList<DefExpr>* symDefs = new AList<DefExpr>();
      for_alist(Stmt, stmt, $2) {
        ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(stmt);
        symDefs->insertAtTail(dynamic_cast<DefExpr*>(exprStmt->expr));
      }
      $$ = new LetExpr(symDefs, $4);
    }
| reduction %prec TREDUCE
| expr TCOLON type
    {
      $$ = new CastExpr($1, $3);
    }
| expr TCOLON STRINGLITERAL
  { 
    $$ = new CallExpr("_tostring", $1, new_StringLiteral($3));
  }
| range %prec TDOTDOT
| seq_expr
| TPLUS expr %prec TUPLUS
    { $$ = new CallExpr("+", $2); }
| TMINUS expr %prec TUMINUS
    { $$ = new CallExpr("-", $2); }
| TNOT expr
    { $$ = new CallExpr("not", $2); }
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
| expr TMOD expr
    { $$ = new CallExpr("mod", $1, $3); }
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
    { $$ = new CallExpr("and", $1, $3); }
| expr TOR expr
    { $$ = new CallExpr("or", $1, $3); }
| expr TEXP expr
    { $$ = new CallExpr("**", $1, $3); }
| expr TSEQCAT expr
    { $$ = new CallExpr("#", $1, $3); }
| expr TBY expr
    { $$ = new CallExpr("by", $1, $3); }
;

reduction:
  identifier TREDUCE expr
    { $$ = new ReduceExpr(new UnresolvedSymbol($1), $3, false); }
| identifier TSCAN expr
    { $$ = new ReduceExpr(new UnresolvedSymbol($1), $3, true); }
;


range:
  expr TDOTDOT expr
    { $$ = new CallExpr("_aseq", $1, $3, new_IntLiteral(1)); }
;


literal:
  INTLITERAL
    { $$ = new_IntLiteral(yytext); }
| FLOATLITERAL
    { $$ = new_FloatLiteral(yytext, atof(yytext)); }
| IMAGLITERAL
    {
      yytext[strlen(yytext)-1] = '\0';
      $$ = new CallExpr("complex", new_FloatLiteral("0.0", 0.0), new_FloatLiteral(yytext, atof(yytext)));
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
| TCOBEGIN
    { $$ = BLOCK_COBEGIN; }
;


for_stmt_tag:
  TFOR
    { $$ = FORLOOPSTMT_FOR; }
| TORDERED TFORALL
    { $$ = FORLOOPSTMT_ORDEREDFORALL; }
| TFORALL
    { $$ = FORLOOPSTMT_FORALL; }
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
