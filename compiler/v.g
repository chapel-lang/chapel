/*
  Copyright 2003-4 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

%<
#include "geysa.h"

extern D_ParserTables parser_tables_v;
extern D_Symbol d_symbols_v[];
%>

program : [ ${scope}->kind = D_SCOPE_RECURSIVE; ] top_level_statement* 
{ $$.ast = new AST(AST_block, &$n); };

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
  $$.ast = new AST(AST_in_module, &$n); 
  if ($#2)
    $$.ast->builtin = if1_cannonicalize_string(
      $g->i, ${child 2, 0, 1}->start_loc.s+1, ${child 2, 0, 1}->end-1);
}
  | 'use' ident ';'
[ assert(!"unsupported"); ]
{ $$.ast = new AST(AST_use_module, &$n); }
  | 'export' module_expression (',' module_expression)* ';'
[ assert(!"unsupported"); ]
{ $$.ast = new AST(AST_export, &$n); }
  | 'import' module_expression (',' module_expression)* ';'
[ assert(!"unsupported"); ]
{ $$.ast = new AST(AST_import, &$n); }
  ;
module_expression
  : module_ident 
  | module_ident 'as' module_ident
{ $$.ast = new AST(AST_as, &$n); };
module_ident: qualified_ident ('.' ident)*;

include "c.g"
c_extern_statement
  : 'extern' ('"c"' | '"C"') c_declarator ';' 
{ $$.ast = new AST(AST_extern, &$n); }
  | 'extern' ('"c"' | '"C"') 'include' (string | bracket_string) ';'
[ assert(!"unsupported"); ]
{ $$.ast = new AST(AST_extern_include, &$n); }
  ;

statement 
  : some_statement ';'
[
  if ($n0.scope->kind == D_SCOPE_PARALLEL)
    ${scope} = enter_D_Scope(${scope}, $g->parallel_scope);
]
{ 
  if ($n0.scope->kind == D_SCOPE_PARALLEL)
    $$.ast = new AST(AST_scope, &$n); 
  else
    $$.ast = $0.ast;
}
  | ident ':' ';'
{ $$.ast = new AST(AST_def_ident, &$n); }
  | ';'
  ;

some_statement
  : expression
  | def_function expression
    [ ${scope} = enter_D_Scope(${scope}, $0.saved_scope); ]
    { $$.ast = new AST(AST_def_fun, &$n); }
  | control_flow
  | some_type_statement
  ;

type_statement 
  : some_type_statement ';'
{ $$.ast = $0.ast; }
  | ';'
  ;

some_type_statement
  : unqualified_type_statement where_statement*
  | where_statement+
  ;

unqualified_type_statement
  : 'type' type_definition (',' type_definition)* 
  | 'class' def_type def_type_parameter_list? class_definition? 
{ $$.ast = new AST(AST_def_type, &$n); }
  | 'enum' def_type enum_definition? 
{ $$.ast = new AST(AST_def_type, &$n); }
  ;

type_definition : def_type def_type_parameter_list? ('__name' string)? 
		(':' constraint_type)? ('=' type)? 
{
  $$.ast = new AST(AST_def_type, &$n); 
  if ($#2)
    $$.ast->builtin = if1_cannonicalize_string(
      $g->i, ${child 2, 0, 1}->start_loc.s+1, ${child 2, 0, 1}->end-1);
};
constraint_types: constraint_type (',' constraint_type)*;
constraint_type: parameterized_type
{ $$.ast = new AST(AST_constraint, &$n); };

def_type: ident 
[
  $$.saved_scope = ${scope};
  ${scope} = new_D_Scope(${scope});
  ${scope}->kind = D_SCOPE_RECURSIVE;
];

def_type_parameter_list : '(' (def_type_parameter (',' def_type_parameter)*)? ')' ;
def_type_parameter
  : def_type
{ $$.ast = new AST(AST_def_type_param, &$n); }
  | def_type ':' type $binary_left 700 
{ $$.ast = new AST(AST_def_type_param, &$n); }
  ;

type
  : type vector_type 	$unary_left 800
{ $$.ast = new AST(AST_vector_type, &$n); }
  | type '&'		$unary_left 700
{ $$.ast = new AST(AST_ref_type, &$n); }
  | type '*' type 	$binary_left 600
{ $$.ast = new AST(AST_product_type, &$n); }
  | type '|' type	$binary_left 500
{ $$.ast = new AST(AST_sum_type, &$n); }
  | type '->' type	$binary_left 400
{ $$.ast = new AST(AST_fun_type, &$n); }
  | ident 'of' type	$binary_right 300
{ $$.ast = new AST(AST_tagged_type, &$n); }
  | type type_parameter_list	$unary_left 200
{ $$.ast = new AST(AST_type_application, &$n); }
  | '(' type  ')'
  | class_definition
  | qualified_ident
  | constant
  ;

type_parameter_list : '(' type_param (',' type_param)* ')' ;
type_param: type { $$.ast = new AST(AST_type_param, &$n); };

class_definition : class_modifiers '{' new_class_scope statement* '}' 
[
  $$.saved_scope = ${scope};
  ${scope} = enter_D_Scope(${scope}, $n0.scope);
]
{ $$.ast = new AST(AST_record_type, &$n); };
new_class_scope: [ ${scope} = new_D_Scope(${scope}); ${scope}->kind = D_SCOPE_RECURSIVE; ];

class_modifiers : (class_modifier (',' class_modifiers)*)?;
class_modifier  
  : (':'  | 'inherits') inherits_ident (',' inherits_ident)*
  | (':>' | 'implements') implements_ident (',' implements_ident)*
  | (':+' | 'includes') includes_ident (',' includes_ident)*
  ;
inherits_ident: parameterized_type { $$.ast = new AST(AST_inherits, &$n); };
implements_ident: parameterized_type { $$.ast = new AST(AST_implements, &$n); };
includes_ident: parameterized_type { $$.ast = new AST(AST_includes, &$n); };
parameterized_type
  : qualified_ident 
  | qualified_ident type_parameter_list
{ $$.ast = new AST(AST_type_application, &$n); };

enum_definition : '{' enumerator (',' enumerator)* '}';
enumerator: ident ('=' expression)? 
{ $$.ast = new AST(AST_tagged_type, &$n); };

vector_type : '[' (vector_index (',' vector_index)*)? ']';
vector_index 
  : anyint 
{ $$.ast = new AST(AST_index, &$n); }
  | anyint '..' anyint
{ $$.ast = new AST(AST_index, &$n); }
  ;

where_statement : 'where' where_type (',' where_type)*;
where_type : qualified_ident (':' constraint_types)? ('=' type)?
{ $$.ast = new AST(AST_where, &$n); };

expression
  : constant ("__name" string)?
   { 
     $$.ast = $0.ast; 
     if ($#1)
       $$.ast->builtin = if1_cannonicalize_string(
         $g->i, ${child 1, 0, 1}->start_loc.s+1, ${child 1, 0, 1}->end-1);
   }
  | qualified_ident
  | def_ident expression $right 5100
    { 
      $$.ast = new AST(AST_def_ident, &$n); 
      $$.ast->def_ident_label = 1;
    }
  | 'var' def_ident expression $right 5100
    { 
      $$.ast = new AST(AST_def_ident, &$n); 
      $$.ast->def_ident_label = 1;
      $$.ast->is_var = 1;
    }
  | anon_function expression
    [ ${scope} = enter_D_Scope(${scope}, $0.saved_scope); ]
    { $$.ast = new AST(AST_def_fun, &$n); }
  | pre_operator expression 
    { $$.ast = op_AST($g->i, $n); }
  | expression post_operator 
    { $$.ast = op_AST($g->i, $n); }
  | expression binary_operator expression
    { $$.ast = op_AST($g->i, $n); }
  | expression ('.' $name "op period" | '->' $name "op arrow") symbol_ident $left 9900
    { $$.ast = op_AST($g->i, $n); }
  | square_block
  | paren_block
  | curly_block
  | expression '?' expression ':' expression $right 8600
    { $$.ast = new AST(AST_if, &$n); }
  | 'if' '(' expression ')' expression $right 6000
    { $$.ast = new AST(AST_if, &$n); }
  | 'if' '(' expression ')' expression 'else' expression $right 6100
    { $$.ast = new AST(AST_if, &$n); }
  | 'while' loop_scope '(' expression ')' expression $right 6200
    [ ${scope} = enter_D_Scope(${scope}, $n0.scope); ]
    { $$.ast = loop_AST($n0, $n3, 0, 0, $n5); }
  | 'do' loop_scope expression 'while' expression $right 6300
    [ ${scope} = enter_D_Scope(${scope}, $n0.scope); ]
    { $$.ast = loop_AST($n0, $n4, &$n2, 0, $n2); }
  | 'for' loop_scope '(' expression? ';' expression? ';' expression? ')'
      expression $right 6400
    [ ${scope} = enter_D_Scope(${scope}, $n0.scope); ]
    { $$.ast = loop_AST($n0, $n5, &$n3, &$n7, $n9); }
  | 'with' with_scope expression $right 5100
    [ ${scope} = enter_D_Scope(${scope}, $n0.scope); ]
    { $$.ast = new AST(AST_with, &$n); }
  | 'conc' expression $right 5000
    { $$.ast = new AST(AST_conc, &$n); }
  | 'seq' expression $right 5000
    { $$.ast = new AST(AST_seq, &$n); }
  ;

loop_scope: [ ${scope} = new_D_Scope(${scope}); ]; 
with_scope : qualified_ident (',' qualified_ident)* ':'
[ ${scope} = new_D_Scope(${scope}); ] 
{ $$.ast = new AST(AST_with_scope, &$n); };

symbol_ident: identifier
{ $$.ast = symbol_AST($g->i, &$n); };

def_ident: idpattern ('__name' string)? ':' 
{
  $$.ast = $0.ast;
  if ($#1)
    $$.ast->builtin = if1_cannonicalize_string(
      $g->i, ${child 1, 0, 1}->start_loc.s+1, ${child 1, 0, 1}->end-1);
};

idpattern
  : ident
  | '(' pattern_type? idpattern (',' idpattern)* ')'
{ $$.ast = new AST(AST_pattern, &$n); };
  ;

def_function: qualified_ident pattern+ ('__name' string)? ':' 
[ 
  $$.saved_scope = ${scope}; 
  ${scope} = new_D_Scope(${scope}); 
  ${scope}->kind = D_SCOPE_RECURSIVE;
]
{
  if ($#2)
    $0.ast->builtin = if1_cannonicalize_string(
      $g->i, ${child 2, 0, 1}->start_loc.s+1, ${child 2, 0, 1}->end-1);
};

anon_function: '\\' pattern+ ':' 
[ 
  $$.saved_scope = ${scope}; 
  ${scope} = new_D_Scope(${scope}); 
];

pattern
  : ident (':' constraint_type)?
{ $$.ast = new AST(AST_arg, &$n); }
  | 'var' ident (':' constraint_type)?
{ $$.ast = new AST(AST_arg, &$n); 
  $$.ast->is_var = 1;
}
  | constant
{ $$.ast = new AST(AST_arg, &$n); }
  | '(' pattern_type? (sub_pattern (',' sub_pattern)*)? ')'
{ $$.ast = new AST(AST_pattern, &$n); };
  ;

pattern_type: ':' qualified_ident
{ $$.ast = new AST(AST_pattern_type, &$n); }
  ;

sub_pattern
  : pattern
  | '...' (ident (':' constraint_type)?)?
{ $$.ast = new AST(AST_vararg, &$n); };
  ;

qualified_ident : global? (ident '::')* ident
{ $$.ast = new AST(AST_qualified_ident, &$n); };

global: '::' 
{ $$.ast = new AST(AST_global, &$n); } ;

with_scope : expression (',' expression)* ':' ;

control_flow
  : 'goto' ident 
{ $$.ast = new AST(AST_goto, &$n); }
  | 'continue' ident? 
{ $$.ast = new AST(AST_continue, &$n); }
  | 'break' ident? 
{ $$.ast = new AST(AST_break, &$n); }
  | 'return' expression? 
{ $$.ast = new AST(AST_return, &$n); }
  ;

binary_operator
  : '.*'        $binary_op_left 9900 
  | '*'         $binary_op_left 9600
  | '/'         $binary_op_left 9600
  | '%'         $binary_op_left 9600
  | '+'         $binary_op_left 9500
  | '-'         $binary_op_left 9500
  | '<<'        $binary_op_left 9400
  | '>>'        $binary_op_left 9400
  | '<'         $binary_op_left 9300
  | '<='        $binary_op_left 9300
  | '>'         $binary_op_left 9300
  | '>='        $binary_op_left 9300
  | '=='        $binary_op_left 9200
  | '!='        $binary_op_left 9200
  | '&'         $binary_op_left 9100
  | '^'         $binary_op_left 9000
  | '|'		$binary_op_left 8900
  | '&&'        $binary_op_left 8800
  | '||'        $binary_op_left 8700
  | '='		$binary_op_left 8500
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
  | ',' 	$binary_op_left 4000
  | '->*'       $binary_op_left 9900
  | '^^' 	$binary_op_left 7000
  |     	$binary_op_left 7000
  ;

pre_operator
  : '++' 	 $unary_op_right 9800
  | '--' 	 $unary_op_right 9800
  | '+'          $unary_op_right 9800
  | '-'          $unary_op_right 9800
  | '~'          $unary_op_right 9800
  | '!'          $unary_op_right 9800
  | '*'		 $unary_op_right 9800
  | '&'          $unary_op_right 9800
  | '(' type ')' $unary_op_right 9800
  | 'sizeof'     $unary_op_right 9900
  ;

post_operator
  : '--'	$unary_op_left 9800
  | '++'	$unary_op_left 9800
  | '{' expression '}' $unary_op_left 9850
  [ if (d_ws_before(${parser}, &$n0) != $n0.start_loc.s) ${reject}; ]
  { $0.ast = symbol_AST($g->i, &$n0); }
  | '{' '}' object $unary_op_left 9850
  [ if (d_ws_before(${parser}, &$n0) != $n0.start_loc.s) ${reject}; ]
  { $0.ast = symbol_AST($g->i, &$n0); }
  | '(' expression ')' $unary_op_left 9850
  [ if (d_ws_before(${parser}, &$n0) != $n0.start_loc.s) ${reject}; ]
  { $0.ast = symbol_AST($g->i, &$n0); }
  | '(' ')' null $unary_op_left 9850
  [ if (d_ws_before(${parser}, &$n0) != $n0.start_loc.s) ${reject}; ]
  { $0.ast = symbol_AST($g->i, &$n0); }
  | '[' expression ']' $unary_op_left 9850
  [ if (d_ws_before(${parser}, &$n0) != $n0.start_loc.s) ${reject}; ]
  { $0.ast = symbol_AST($g->i, &$n0); }
  ;

object:
  { $$.ast = new AST(AST_object); }
  ;

null: 
  { $$.ast = new AST(AST_list); }
  ;

curly_block: '{' [ ${scope} = new_D_Scope(${scope}); ${scope}->kind = D_SCOPE_RECURSIVE; ]
              statement* expression? '}'
{ 
  if ($#3)
    $$.ast = new AST(AST_scope, &$n); 
  else
    $$.ast = new AST(AST_object, &$n);
};

paren_block: '(' statement* expression? ')' 
{ 
  if ($#2)
    $$.ast = new AST(AST_block, &$n); 
  else
    $$.ast = new AST(AST_list, &$n);
};

square_block: '[' 
  [ 
    ${scope} = new_D_Scope(${scope}); ${scope}->kind = D_SCOPE_PARALLEL; 
    $g->parallel_scope = ${scope};
  ]
  statement* expression? ']' 
{ 
  if ($#3)
    $$.ast = new AST(AST_scope, &$n); 
  else
    $$.ast = new AST(AST_vector, &$n);
};

constant : (char | int8 | uint8 | int16 | uint16 | 
	    int32 | uint32 | int64 | uint64 | int | uint |
	    float32 | float64 | float128 | float | 
	    string | symbol) ('__name' string)?
{ 
  $$.ast = new AST(AST_const, &$n); 
  $$.ast->string = if1_cannonicalize_string($g->i, $n0.start_loc.s, $n0.end);
  $$.ast->constant_type = constant_type($n, d_symbols_v);
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
char ::= "\'([^\'\\]|\\[^])*\'";

base_int ::= "(0[0-7]*|(0x|0X)[0-9a-fA-F]+|[1-9][0-9]*)";
int8	 ::= base_int "(b|B)";
uint8	 ::= base_int "(u|U)(b|B)|(b|B)(u|U)";
int16	 ::= base_int "(s|S)";
uint16	 ::= base_int "(u|U)(s|S)|(s|S)(u|U)";
int32	 ::= base_int "(w|W)";
uint32	 ::= base_int "(u|U)(w|W)|(w|W)(u|U)";
int64	 ::= base_int "(l|L)";
uint64	 ::= base_int "(u|U)(l|L)|(u|U)(l|L)";
int	 ::= base_int  "/[^\.]";
uint	 ::= base_int "(u|U)";
anyint	 : char | int8 | uint8 | int16 | uint16 | 
	   int32 | uint32 | int64 | uint64 | int | uint
{ 
  $$.ast = new AST(AST_const); 
  $$.ast->string = if1_cannonicalize_string($g->i, $n0.start_loc.s, $n0.end);
  $$.ast->constant_type = constant_type($n, d_symbols_v);
};

base_float ::= "(([0-9]+.[0-9]*|[0-9]*.[0-9]+)([eE][\-\+]?[0-9]+)?|[0-9]+[eE][\-\+]?[0-9]+)";
float32	   ::= base_float "(f|F)";
float64	   ::= base_float "(d|D)";
float80	   ::= base_float "(t|T)";
float128   ::= base_float "(l|L)";
float	   ::= base_float "/[^\.]";

identifier : "[a-zA-Z_][a-zA-Z0-9_]*" $term -1;

ident : identifier{ 
  $$.ast = new AST(AST_ident, &$n); 
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
