/*
  Copyright 2003-4 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

%<
#include "parse.h"

extern D_Symbol d_symbols_chpl[];

static ParseAST *def_class(D_ParseNode *pn) {
  ParseAST *a = new_AST(AST_def_type, pn); 
  D_ParseNode *p = d_get_child(d_get_child(pn, 0), 0);
  if (p->symbol == ${string value})
    a->is_value = 1;
  return a;
}
%>

program : [ ${scope}->kind = D_SCOPE_RECURSIVE; ] top_level_statement* 
{ $$.ast = new_AST(AST_block, &$n); };

top_level_statement: some_top_level_statement
[ ${scope} = commit_D_Scope(${scope}); ];

some_top_level_statement
  : module_statement 
  | c_extern_statement
  | statement
  ;

module_statement 
  : 'in' ident ('__name' string)? ';' 
[ in_module($g, $n1.start_loc.s, $n1.end, &${scope}); ]
{ 
  $$.ast = new_AST(AST_in_module, &$n); 
  if ($#2)
    $$.ast->builtin = if1_cannonicalize_string(
      $g->i, ${child 2, 0, 1}->start_loc.s+1, ${child 2, 0, 1}->end-1);
}
  | 'use' ident ';'
[ assert(!"unsupported"); ]
{ $$.ast = new_AST(AST_use_module, &$n); }
  | 'export' module_expression (',' module_expression)* ';'
[ assert(!"unsupported"); ]
{ $$.ast = new_AST(AST_export, &$n); }
  | 'import' module_expression (',' module_expression)* ';'
[ assert(!"unsupported"); ]
{ $$.ast = new_AST(AST_import, &$n); }
  ;
module_expression
  : module_ident 
  | module_ident 'as' module_ident
{ $$.ast = new_AST(AST_as, &$n); };
module_ident: qualified_ident ("\./[^\.\*]" ident)*;

include "c.g"
c_extern_statement
  : 'extern' ('"c"' | '"C"') c_declarator ';' 
{ $$.ast = new_AST(AST_extern, &$n); }
  | 'extern' ('"c"' | '"C"') 'include' (string | bracket_string) ';'
[ assert(!"unsupported"); ]
{ $$.ast = new_AST(AST_extern_include, &$n); }
  ;

statement 
  : some_statement ';'
[
  if ($n0.scope->kind == D_SCOPE_PARALLEL)
    ${scope} = enter_D_Scope(${scope}, $g->parallel_scope);
]
{ 
  if ($n0.scope->kind == D_SCOPE_PARALLEL)
    $$.ast = new_AST(AST_scope, &$n); 
  else
    $$.ast = $0.ast;
}
  | def_function function_body
    [ ${scope} = enter_D_Scope(${scope}, $0.saved_scope); ]
    { $$.ast = new_AST(AST_def_fun, &$n); }
  | class def_type def_type_parameter_list? class_definition
{ $$.ast = def_class(&$n); }
  | class def_type def_type_parameter_list? ';'
{ $$.ast = def_class(&$n); }
  | ident ':' ';'
{ $$.ast = new_AST(AST_label, &$n); }
  | ';'
  ;

class 
  : 'class'
  | 'value'
  ;

function_body: '{' statement* expression? '}'
{ $$.ast = new_AST(AST_scope, &$n); };

some_statement
  : expression
  | var_declarations
  | control_flow
  | some_type_statement
  ;

var_declarations 
  : 'var' var_declaration_list
{ Vec<ParseAST *> asts; get_ast(asts, &$n); forv_ParseAST(a, asts) a->is_var = 1; }
  | 'const'  var_declaration_list
{ Vec<ParseAST *> asts; get_ast(asts, &$n); forv_ParseAST(a, asts) a->is_const = 1; }
  | 'let' var_declaration_list
  ;
var_declaration_list :  var_declaration (',' var_declaration)*;
var_declaration : ident ('__name' string)? (':' constraint_type)? ('=' expression)?
{
  $$.ast = new_AST(AST_def_ident, &$n); 
  if ($#1)
    $$.ast->builtin = if1_cannonicalize_string(
      $g->i, ${child 1, 0, 1}->start_loc.s+1, ${child 1, 0, 1}->end-1);
};

pure_type_statement 
  : some_pure_type_statement ';'
{ $$.ast = $0.ast; }
  | class def_type def_type_parameter_list? class_definition
{ $$.ast = def_class(&$n); }
  | class def_type def_type_parameter_list? ';'
{ $$.ast = def_class(&$n); }
  | ';'
  ;

some_type_statement
  : unqualified_type_statement where_statement*
  | where_statement+
  ;

some_pure_type_statement
  : unqualified_pure_type_statement where_statement*
  | where_statement+
  ;

unqualified_type_statement
  : 'type' type_definition (',' type_definition)* 
  | 'enum' def_type enum_definition? 
{ $$.ast = new_AST(AST_def_type, &$n); }
  ;

unqualified_pure_type_statement
  : def_ident type
{  $$.ast = new_AST(AST_declare_ident, &$n); }
  | unqualified_type_statement
  | var_declarations
  | def_function ':' type
[ ${scope} = enter_D_Scope(${scope}, $0.saved_scope); ]
{ $$.ast = new_AST(AST_def_fun, &$n); }
  ;

type_definition : def_type def_type_parameter_list? ('__name' string)? 
		(':' constraint_type)? ('=' type)? 
{
  $$.ast = new_AST(AST_def_type, &$n); 
  if ($#2)
    $$.ast->builtin = if1_cannonicalize_string(
      $g->i, ${child 2, 0, 1}->start_loc.s+1, ${child 2, 0, 1}->end-1);
};
constraint_types: constraint_type (',' constraint_type)*;
constraint_type: parameterized_type
{ $$.ast = new_AST(AST_constraint, &$n); };

def_type: ident 
[
  $$.saved_scope = ${scope};
  ${scope} = new_D_Scope(${scope});
  ${scope}->kind = D_SCOPE_RECURSIVE;
];

def_type_parameter_list : '(' (def_type_parameter (',' def_type_parameter)*)? ')' ;
def_type_parameter
  : def_type
{ $$.ast = new_AST(AST_def_type_param, &$n); }
  | def_type ':' type $binary_left 700 
{ $$.ast = new_AST(AST_def_type_param, &$n); }
  ;

type
  : type vector_type 	$unary_left 800
{ $$.ast = new_AST(AST_vector_type, &$n); }
  | type '&'		$unary_left 700
{ $$.ast = new_AST(AST_ref_type, &$n); }
  | type '*' type 	$binary_left 600
{ $$.ast = new_AST(AST_product_type, &$n); }
  | type '|' type	$binary_left 500
{ $$.ast = new_AST(AST_sum_type, &$n); }
  | type '->' type	$binary_left 400
{ $$.ast = new_AST(AST_fun_type, &$n); }
  | ident 'of' type	$binary_right 300
{ $$.ast = new_AST(AST_tagged_type, &$n); }
  | type type_parameter_list	$unary_left 200
{ $$.ast = new_AST(AST_type_application, &$n); }
  | '(' type  ')'
  | class_definition
  | qualified_ident
  | constant
  ;

type_parameter_list : '(' type_param (',' type_param)* ')' ;
type_param: type { $$.ast = new_AST(AST_type_param, &$n); };

class_definition : class_modifiers '{' new_class_scope pure_type_statement* '}' 
[
  $$.saved_scope = ${scope};
  ${scope} = enter_D_Scope(${scope}, $n0.scope);
]
{ $$.ast = new_AST(AST_record_type, &$n); };
new_class_scope: [ ${scope} = new_D_Scope(${scope}); ${scope}->kind = D_SCOPE_RECURSIVE; ];

class_modifiers : (class_modifier (',' class_modifiers)*)?;
class_modifier  
  : (':'  | 'inherits') inherits_ident (',' inherits_ident)*
  | (':>' | 'implements') implements_ident (',' implements_ident)*
  | (':+' | 'includes') includes_ident (',' includes_ident)*
  ;
inherits_ident: parameterized_type { $$.ast = new_AST(AST_inherits, &$n); };
implements_ident: parameterized_type { $$.ast = new_AST(AST_implements, &$n); };
includes_ident: parameterized_type { $$.ast = new_AST(AST_includes, &$n); };
parameterized_type
  : qualified_ident 
  | array_descriptor $right 100
  | array_descriptor parameterized_type $unary_right 200
  | parameterized_type type_parameter_list $unary_left 300
{ $$.ast = new_AST(AST_type_application, &$n); };

array_descriptor : '[' (indices_colon? domain)? ']'
{ $$.ast = new_AST(AST_array_descriptor, &$n); };

indices_colon : indices ':';

indices : ident (',' ident)*
{ $$.ast = new_AST(AST_indices, &$n); };

enum_definition : '{' enumerator (',' enumerator)* '}';
enumerator: ident ('=' expression)? 
{ $$.ast = new_AST(AST_tagged_type, &$n); };

vector_type : '[' (vector_index (',' vector_index)*)? ']';
vector_index 
  : anyint 
{ $$.ast = new_AST(AST_index, &$n); }
  | anyint '..' anyint ('by' anyint)?
{ $$.ast = new_AST(AST_index, &$n); }
  ;

where_statement : 'where' where_type (',' where_type)*;
where_type : qualified_ident (':' constraint_types)? ('=' type)?
{ $$.ast = new_AST(AST_where, &$n); };

expression
  : constant ("__name" string)?
   { 
     $$.ast = $0.ast; 
     if ($#1)
       $$.ast->builtin = if1_cannonicalize_string(
         $g->i, ${child 1, 0, 1}->start_loc.s+1, ${child 1, 0, 1}->end-1);
   }
  | 'new' expression
    { $$.ast = new_AST(AST_new, &$n); }
  | qualified_ident
  | def_ident expression $right 5100
    { 
      $$.ast = new_AST(AST_def_ident, &$n); 
      $$.ast->def_ident_label = 1;
    }
  | anon_function '{' statement* expression? '}'
    [ ${scope} = enter_D_Scope(${scope}, $0.saved_scope); ]
    { $$.ast = new_AST(AST_def_fun, &$n); }
  | pre_operator expression 
    { $$.ast = op_AST($g->i, $n); }
  | expression post_operator 
    { $$.ast = op_AST($g->i, $n); }
  | expression binary_operator expression
    { $$.ast = op_AST($g->i, $n); }
  | expression ("\./[^\.\*]" $name "op period" | '->' $name "op arrow") symbol_ident $left 9900
    { $$.ast = op_AST($g->i, $n); }
  | expression '..' expression ('by' expression)? $left 8400
    { $$.ast = new_AST(AST_index, &$n); }
  | vector_immediate
  | list_immediate
  | paren_block
  | curly_block
  | square_forall
  | cross_product
  | 'forall' sub_scope indices 'in' domain curly_block
    [ ${scope} = enter_D_Scope(${scope}, $n0.scope); ]
    { $$.ast =  new_AST(AST_forall, &$n); }
  | expression '?' expression ':' expression $right 8600
    { $$.ast = new_AST(AST_if, &$n); }
  | 'if' expression 'then' expression $right 6000
    { $$.ast = new_AST(AST_if, &$n); }
  | 'if' expression 'then' expression 'else' expression $right 6100
    { $$.ast = new_AST(AST_if, &$n); }
  | 'while' sub_scope expression 'do' expression $right 6200
    [ ${scope} = enter_D_Scope(${scope}, $n0.scope); ]
    { $$.ast = loop_AST($n0, $n2, 0, 0, $n4); }
  | 'do' sub_scope expression 'while' expression $right 6300
    [ ${scope} = enter_D_Scope(${scope}, $n0.scope); ]
    { $$.ast = loop_AST($n0, $n4, &$n2, 0, $n2); }
  | 'for' sub_scope '(' expression? ';' expression? ';' expression? ')'
      expression $right 6400
    [ ${scope} = enter_D_Scope(${scope}, $n0.scope); ]
    { $$.ast = loop_AST($n0, $n5, &$n3, &$n7, $n9); }
  | 'with' with_scope expression $right 5100
    [ ${scope} = enter_D_Scope(${scope}, $n0.scope); ]
    { $$.ast = new_AST(AST_with, &$n); }
  | 'conc' expression $right 5000
    { $$.ast = new_AST(AST_conc, &$n); }
  | 'seq' expression $right 5000
    { $$.ast = new_AST(AST_seq, &$n); }
  ;

sub_scope: [ ${scope} = new_D_Scope(${scope}); ]; 
with_scope : expression (',' expression)* ':'
[ ${scope} = new_D_Scope(${scope}); ] 
{ $$.ast = new_AST(AST_with_scope, &$n); };

symbol_ident: identifier
{ $$.ast = symbol_AST($g->i, &$n); };

def_ident: ident ('__name' string)? ':' 
{
  $$.ast = $0.ast;
  if ($#1)
    $$.ast->builtin = if1_cannonicalize_string(
      $g->i, ${child 1, 0, 1}->start_loc.s+1, ${child 1, 0, 1}->end-1);
};

def_function: 'function' qualified_ident pattern+ ('__name' string)?
[ 
  $$.saved_scope = ${scope}; 
  ${scope} = new_D_Scope(${scope}); 
  ${scope}->kind = D_SCOPE_RECURSIVE;
]
{
  if ($#3)
    $1.ast->builtin = if1_cannonicalize_string(
      $g->i, ${child 3, 0, 1}->start_loc.s+1, ${child 3, 0, 1}->end-1);
};

anon_function: 'fun' pattern+
[ 
  $$.saved_scope = ${scope}; 
  ${scope} = new_D_Scope(${scope}); 
];

pattern
  : ident (':' constraint_type)?
{ $$.ast = new_AST(AST_arg, &$n); }
  | 'var' ident (':' constraint_type)?
{ $$.ast = new_AST(AST_arg, &$n); 
  $$.ast->is_var = 1;
}
  | constant
{ $$.ast = new_AST(AST_arg, &$n); }
  | '(' (sub_pattern (',' sub_pattern)*)? ')'
{ $$.ast = new_AST(AST_pattern, &$n); };
  ;

sub_pattern
  : pattern
  | '...' (ident (':' constraint_type)?)?
{ $$.ast = new_AST(AST_vararg, &$n); };
  ;

qualified_ident : global? (ident '::')* ident
{ $$.ast = new_AST(AST_qualified_ident, &$n); };

global: '::' 
{ $$.ast = new_AST(AST_global, &$n); } ;

with_scope : expression (',' expression)* ':' ;

control_flow
  : 'goto' ident 
{ $$.ast = new_AST(AST_goto, &$n); }
  | 'continue' ident? 
{ $$.ast = new_AST(AST_continue, &$n); }
  | 'break' ident? 
{ $$.ast = new_AST(AST_break, &$n); }
  | 'return' expression? 
{ $$.ast = new_AST(AST_return, &$n); }
  ;

binary_operator
  : '.*'        $binary_op_left 9900 
  | '**'        $binary_op_right 9700
  | "\*/[^*]"	$binary_op_left 9600
  | "\//[^=]"	$binary_op_left 9600
  | '%'         $binary_op_left 9600
  | "\+/[^+]"	$binary_op_left 9500
  | "-/[^\-]"	$binary_op_left 9500
  | "<</[^=]"	$binary_op_left 9400
  | ">>/[^="	$binary_op_left 9400
  | "</[^<=]"	$binary_op_left 9300
  | '<='        $binary_op_left 9300
  | ">/[^>=]"   $binary_op_left 9300
  | '>='        $binary_op_left 9300
  | '=='        $binary_op_left 9200
  | '!='        $binary_op_left 9200
  | "&/[^&]"    $binary_op_left 9100
  | "^/[^^]"	$binary_op_left 9000
  | "\|/[^|]"	$binary_op_left 8900
  | '&&'        $binary_op_left 8800
  | '||'        $binary_op_left 8700
  | "=/[^=]"	$binary_op_left 8500
  | '*='        $binary_op_left 8500
  | '/='        $binary_op_left 8500
  | '%='        $binary_op_left 8500
  | '+='        $binary_op_left 8500
  | '-='        $binary_op_left 8500
  | '<<='       $binary_op_left 8500
  | '>>='       $binary_op_left 8500
  | '&='        $binary_op_left 8500
  | '|='        $binary_op_left 8500
  | '^='        $binary_op_left 8500
  | ',' 	$binary_op_left 8300 
  | '->*'       $binary_op_left 9900
  | '^^' 	$binary_op_left 7000
  ;

pre_operator
  : '++' 	 $unary_op_right 9800
  | '--' 	 $unary_op_right 9800
  | "\+/[^+]"	 $unary_op_right 9800
  | "-/[^\-]"	 $unary_op_right 9800
  | '~'          $unary_op_right 9800
  | '!'          $unary_op_right 9800
  | "\*/[^*]"	 $unary_op_right 9800
  | "&/[^&]"	 $unary_op_right 9800
  | '(' type ')' $unary_op_right 9800
  | 'sizeof'     $unary_op_right 9900
  ;

post_operator
  : '--'	$unary_op_left 9800
  | '++'	$unary_op_left 9800
  | '(' expression_or_null ')' $unary_op_left 9850
  [ if (d_ws_before(${parser}, &$n0) != $n0.start_loc.s) ${reject}; ]
  { $0.ast = symbol_AST($g->i, &$n0); }
  ;

expression_or_null
  : expression
  |
{
  $$.ast = new_AST(AST_qualified_ident, &$n); 
  ParseAST *a = new_AST(AST_global);
  a = new_AST(AST_ident);
  a->string = if1_cannonicalize_string($g->i, "null");
  $$.ast->add(a);
}
  ;

curly_block: '{' [ ${scope} = new_D_Scope(${scope}); ${scope}->kind = D_SCOPE_RECURSIVE; ]
              statement* expression? '}'
{ 
  if ($#3)
    $$.ast = new_AST(AST_scope, &$n); 
  else
    $$.ast = new_AST(AST_object, &$n);
};

paren_block: '(' statement* expression? ')' 
{ 
  if ($#2)
    $$.ast = new_AST(AST_block, &$n); 
  else
    $$.ast = new_AST(AST_list, &$n);
};

list_immediate: '#' '(' statement* expression? ')' { 
  $$.ast = new_AST(AST_list, &$n);
};

vector_immediate: '#' '[' statement* expression? ']' { 
  $$.ast = new_AST(AST_vector, &$n);
};

square_forall: '[' sub_scope indices_colon? domain ']' expression
[ ${scope} = enter_D_Scope(${scope}, $n0.scope); ]
{ $$.ast = new_AST(AST_forall, &$n); };

cross_product : '[' sub_scope expression ']'
[ ${scope} = enter_D_Scope(${scope}, $n0.scope); ]
{ $$.ast = new_AST(AST_cross_product, &$n); };

domain: expression 
{ $$.ast = new_AST(AST_domain, &$n); };

constant : (character | int8 | uint8 | int16 | uint16 | 
	    int32 | uint32 | int64 | uint64 | int | uint |
	    float32 | float64 | float128 | float | 
	    string | symbol) ('__name' string)?
{ 
  $$.ast = new_AST(AST_const, &$n); 
  $$.ast->string = if1_cannonicalize_string($g->i, $n0.start_loc.s, $n0.end);
  $$.ast->constant_type = constant_type($n, d_symbols_chpl);
  if ($#1)
    $$.ast->builtin = if1_cannonicalize_string(
      $g->i, ${child 1, 0, 1}->start_loc.s+1, ${child 1, 0, 1}->end-1);
};

symbol 
  : '#' identifier
  | '#' string
  ;

string ::= "\"([^\"\\]|\\[^])*\"";
bracket_string ::= "<([^\>\\]|\\[^])*>";
character ::= "\'([^\'\\]|\\[^])*\'";

base_int ::= "(0[0-7]*|(0x|0X)[0-9a-fA-F]+|[1-9][0-9]*)";
int8	 ::= base_int "(b|B)";
uint8	 ::= base_int "(u|U)(b|B)|(b|B)(u|U)";
int16	 ::= base_int "(s|S)";
uint16	 ::= base_int "(u|U)(s|S)|(s|S)(u|U)";
int32	 ::= base_int "(w|W)";
uint32	 ::= base_int "(u|U)(w|W)|(w|W)(u|U)";
int64	 ::= base_int "(l|L)";
uint64	 ::= base_int "(u|U)(l|L)|(u|U)(l|L)";
int	 ::= base_int;
uint	 ::= base_int "(u|U)";
anyint	 : character | int8 | uint8 | int16 | uint16 | 
           int32 | uint32 | int64 | uint64 | int | uint
{ 
  $$.ast = new_AST(AST_const); 
  $$.ast->string = if1_cannonicalize_string($g->i, $n0.start_loc.s, $n0.end);
  $$.ast->constant_type = constant_type($n, d_symbols_chpl);
};

base_float ::= "(([0-9]+.[0-9]*|[0-9]*.[0-9]+)([eE][\-\+]?[0-9]+)?|[0-9]+[eE][\-\+]?[0-9]+)";
float32	   ::= base_float "(f|F)";
float64	   ::= base_float "(d|D)";
float80	   ::= base_float "(t|T)";
float128   ::= base_float "(l|L)";
float	   ::= base_float "/[^\.]";

anynum ::= character | int8 | uint8 | int16 | uint16 | 
           int32 | uint32 | int64 | uint64 | int | uint |
           float32 | float64 | float128 | float;

identifier : "[a-zA-Z_][a-zA-Z0-9_]*" $term -1;

ident : identifier { 
  $$.ast = new_AST(AST_ident, &$n); 
  $$.ast->string = if1_cannonicalize_string($g->i, $n0.start_loc.s, $n0.end);
};

 _ : 
[
  if ($# == 1)
    $$.saved_scope = $0.saved_scope;
]
{
  if ($# == 1)
    $$.ast = $0.ast;
};
