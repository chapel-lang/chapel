%<
#include "geysa.h"
#include "parse_structs.h"
#include "grammar.h"
#include "parse_ast.h"
#include "if1.h"

extern D_ParserTables parser_tables_v;
extern D_Symbol d_symbols_v[];
%>

program : top_level_statement* { 
  $$.ast = new_AST(AST_block, &$n); 
  $$.ast->scope_kind = Scope_RECURSIVE; 
};

top_level_statement
  : module_statement 
  | c_extern_statement
  | statement
  ;

module_statement 
  : 'in' ident ('__name' string)? ';' 
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

module_ident: qualified_ident ('.' ident)*;

include "c.g"
c_extern_statement
  : 'extern' ('"c"' | '"C"') c_declarator ';' 
    { $$.ast = new_AST(AST_extern, &$n); }
  | 'extern' ('"c"' | '"C"') 'include' (string | bracket_string) ';'
    [ assert(!"unsupported"); ]
    { $$.ast = new_AST(AST_extern_include, &$n); }
  ;

statement
  : unterminated_statement ';'
  | ';'
  ;

unterminated_statement
  : expression
  | def_function unterminated_statement $right 1000
    { $$.ast = new_AST(AST_def_fun, &$n); 
      $$.ast->scope_kind = Scope_RECURSIVE; }
  | control_flow
  | type_statement
  | expression '=' expression $left 3000
    { $$.ast = op_AST($g->i, $n); }
  | 'var' def_ident_var_list $right 5100
  | 'with' with_scope unterminated_statement $right 5100
    { $$.ast = new_AST(AST_with, &$n); }
  ;

type_statement
  : unqualified_type_statement where_statement*
  | where_statement+
  ;

unqualified_type_statement
  : ('type' | 'subtype') type_definition (',' type_definition)* 
  | 'class' ident def_type_parameter_list? class_definition? 
    { $$.ast = new_AST(AST_def_type, &$n); 
      $$.ast->scope_kind = Scope_RECURSIVE; }
  | 'enum' ident enum_definition? 
    { $$.ast = new_AST(AST_def_type, &$n); 
      $$.ast->scope_kind = Scope_RECURSIVE; }
  ;

type_definition : ident def_type_parameter_list? ('__name' string)? 
		(':' must_implement)? ('=' type)? {
  $$.ast = new_AST(AST_def_type, &$n); 
  if ($#2)
    $$.ast->builtin = if1_cannonicalize_string(
      $g->i, ${child 2, 0, 1}->start_loc.s+1, ${child 2, 0, 1}->end-1);
  $$.ast->scope_kind = Scope_RECURSIVE;
};
must_implement: must_implement_type (',' must_implement_type)*;
must_implement_type: parameterized_type
  { $$.ast = new_AST(AST_must_implement, &$n); };

def_type_parameter_list : '(' (def_type_parameter (',' def_type_parameter)*)? ')' ;
def_type_parameter
  : ident
    { $$.ast = new_AST(AST_def_type_param, &$n); }
  | ident ':' type $binary_left 700 
    { $$.ast = new_AST(AST_def_type_param, &$n); }
  ;

type
  : type vector_type 	$unary_left 800
    { $$.ast = new_AST(AST_vector_type, &$n); }
  | type '*'		$unary_left 700
    { $$.ast = new_AST(AST_ref_type, &$n); }
  | type '*' type 	$binary_left 600
    { $$.ast = new_AST(AST_product_type, &$n); }
  | type '|' type	$binary_left 500
    { $$.ast = new_AST(AST_lub_type, &$n); }
  | type '&' type	$binary_left 500
    { $$.ast = new_AST(AST_glb_type, &$n); }
  | type '->' type	$binary_left 400
    { $$.ast = new_AST(AST_fun_type, &$n); }
  | ident 'of' type	$binary_right 300
    { $$.ast = new_AST(AST_tagged_type, &$n); }
  | type type_parameter_list $unary_left 200
    { $$.ast = new_AST(AST_type_application, &$n); }
  | '(' type  ')'
  | class_definition
  | qualified_ident
  | constant
  ;

type_parameter_list : '(' type_param (',' type_param)* ')' ;
type_param: type { $$.ast = new_AST(AST_type_param, &$n); };

class_definition : class_modifiers '{' statement* '}' { 
  $$.ast = new_AST(AST_record_type, &$n); 
  $$.ast->scope_kind = Scope_RECURSIVE; 
};

class_modifiers : (class_modifier (',' class_modifiers)*)?;
class_modifier  
  : (':'  | 'inherits') inherits_ident (',' inherits_ident)*
  | ('>' | 'implements') implements_ident (',' implements_ident)*
  | ('@' | 'specializes') specializes_ident (',' specializes_ident)*
  | ('+' | 'includes') includes_ident (',' includes_ident)*
  ;
inherits_ident: parameterized_type { $$.ast = new_AST(AST_inherits, &$n); };
implements_ident: parameterized_type { $$.ast = new_AST(AST_implements, &$n); };
specializes_ident: parameterized_type { $$.ast = new_AST(AST_specializes, &$n); };
includes_ident: parameterized_type { $$.ast = new_AST(AST_includes, &$n); };
parameterized_type
  : qualified_ident 
  | qualified_ident type_parameter_list
    { $$.ast = new_AST(AST_type_application, &$n); };

enum_definition : '{' enumerator (',' enumerator)* '}';
enumerator: ident ('=' expression)? { 
  $$.ast = new_AST(AST_tagged_type, &$n); 
};

vector_type : '[' (vector_index (',' vector_index)*)? ']';
vector_index 
  : anyint 
    { $$.ast = new_AST(AST_index, &$n); }
  | anyint '..' anyint
    { $$.ast = new_AST(AST_index, &$n); }
  ;

where_statement : 'where' where_type (',' where_type)*;
where_type : qualified_ident (':' must_implement)? ('=' type)?
  { $$.ast = new_AST(AST_where, &$n); };

expression
  : constant ("__name" string)? { 
     $$.ast = $0.ast; 
     if ($#1)
       $$.ast->builtin = if1_cannonicalize_string(
         $g->i, ${child 1, 0, 1}->start_loc.s+1, ${child 1, 0, 1}->end-1);
   }
  | qualified_ident
  | def_ident expression $right 5100 { 
      $$.ast = new_AST(AST_def_ident, &$n); 
      $$.ast->def_ident_label = 1;
    }
  | 'let' def_ident_list ('in' expression)? $right 5100 {
      if ($#2)
        $$.ast = new_AST(AST_scope, &$n); 
    }
  | anon_function expression { 
      $$.ast = new_AST(AST_def_fun, &$n);
      $$.ast->scope_kind = Scope_RECURSIVE; 
    }
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
    { $$.ast = new_AST(AST_ifexpr, &$n); }
  | 'if' expression 'then' unterminated_statement $right 2000
    { $$.ast = new_AST(AST_ifexpr, &$n); }
  | 'if' expression 'then' unterminated_statement 'else' unterminated_statement $right 2100
    { $$.ast = new_AST(AST_ifexpr, &$n); }
  | 'conc' expression $right 5000
    { $$.ast = new_AST(AST_conc, &$n); }
  | 'seq' expression $right 5000
    { $$.ast = new_AST(AST_seq, &$n); }
  ;

init_expression : expression
  { $$.ast = new_AST(AST_init, &$n); }
  ;

with_scope : qualified_ident (',' qualified_ident)* ':'
  { $$.ast = new_AST(AST_with_scope, &$n); };

symbol_ident: identifier
  { $$.ast = symbol_AST($g->i, &$n); };

def_ident: idpattern ('__name' string)? ':' {
  $$.ast = $0.ast;
  if ($#1)
    $$.ast->builtin = if1_cannonicalize_string(
      $g->i, ${child 1, 0, 1}->start_loc.s+1, ${child 1, 0, 1}->end-1);
};

var_ident: idpattern ('__name' string)? '=' {
  $$.ast = $0.ast;
  if ($#1)
    $$.ast->builtin = if1_cannonicalize_string(
      $g->i, ${child 1, 0, 1}->start_loc.s+1, ${child 1, 0, 1}->end-1);
};

def_ident_one
  : def_ident expression 
    { $$.ast = new_AST(AST_def_ident, &$n); }
  | def_function unterminated_statement
    { $$.ast = new_AST(AST_def_fun, &$n); 
      $$.ast->scope_kind = Scope_RECURSIVE; }
  ; 
def_ident_list
  : def_ident_list ',' def_ident_list $left 10000
  | def_ident_one;
  ;

def_ident_var_one
  : var_ident expression { 
      $$.ast = new_AST(AST_def_ident, &$n); 
      $$.ast->add(new_AST(AST_var));
    }
  | idpattern {
      $$.ast = new_AST(AST_def_ident, &$n); 
      $$.ast->add(new_AST(AST_var));
    }
  ;
def_ident_var_list
  : def_ident_var_list ',' def_ident_var_list $left 10000
  | def_ident_var_one;
  ;

idpattern
  : ident (':' identifier)?
    { 
      $$.ast = $0.ast;
      if ($#1)
        $$.ast->alt_name = if1_cannonicalize_string(
          $g->i, ${child 1, 0, 1}->start_loc.s, ${child 1, 0, 1}->end);
    }
  | '(' pattern_type? idpattern (',' idpattern)* ')'
    { $$.ast = new_AST(AST_pattern, &$n); };
  ;

def_function: qualified_ident pattern+ ('__name' string)? ':' {
  if ($#2)
    $0.ast->builtin = if1_cannonicalize_string(
      $g->i, ${child 2, 0, 1}->start_loc.s+1, ${child 2, 0, 1}->end-1);
};

anon_function: '\\' pattern+ ':';

must_specialize_type: parameterized_type
  { $$.ast = new_AST(AST_must_specialize, &$n); };

must_implement_and_specialize_type: parameterized_type nothing
  { ParseAST *ast = $0.ast;
    $0.ast = new_AST(AST_must_implement);
    $0.ast->add((ParseAST*)ast->copy());
    $1.ast = new_AST(AST_must_specialize);
    $1.ast->add(ast);
  };

nothing : ;

pattern
  : ident
    { $$.ast = new_AST(AST_arg, &$n); }
  | constant
    { $$.ast = new_AST(AST_arg, &$n); }
  | '(' pattern_type? (sub_pattern (',' sub_pattern)*)? ')'
    { $$.ast = new_AST(AST_pattern, &$n); };
  ;

pattern_type: ':' qualified_ident
    { $$.ast = new_AST(AST_pattern_type, &$n); }
  ;

pattern_suffix :
  ('@' must_specialize_type)? ('<' must_implement_type)? 
  (':' must_implement_and_specialize_type)?  ('=' init_expression)?;

sub_pattern
  : intent? 'var'? ident pattern_suffix
    { 
      $$.ast = new_AST(AST_arg, &$n); 
      if ($#1) $$.ast->add(new_AST(AST_var));
    }
  | constant
    { $$.ast = new_AST(AST_arg, &$n); }
  | '...' (intent? ident pattern_suffix)?
    { $$.ast = new_AST(AST_rest, &$n); };
  ;

intent
  : 'const'
    { $$.ast = new_AST(AST_intent, &$n); 
      $$.ast->intent = Intent_const; }
  | 'in'
    { $$.ast = new_AST(AST_intent, &$n); 
      $$.ast->intent = Intent_in; }
  | 'out'
    { $$.ast = new_AST(AST_intent, &$n); 
      $$.ast->intent = Intent_out; }
  | 'inout'
    { $$.ast = new_AST(AST_intent, &$n); 
      $$.ast->intent = Intent_inout; }
  ;

qualified_ident : global? (ident '::')* ident
  { $$.ast = new_AST(AST_qualified_ident, &$n); };

global: '::' 
  { $$.ast = new_AST(AST_global, &$n); } ;

control_flow
  : 'goto' ident 
    { $$.ast = new_AST(AST_goto, &$n); }
  | 'continue' ident? 
    { $$.ast = new_AST(AST_continue, &$n); }
  | 'break' ident? 
    { $$.ast = new_AST(AST_break, &$n); }
  | 'return' expression? 
    { $$.ast = new_AST(AST_return, &$n); }
  | 'while' '(' expression ')' unterminated_statement $right 2200
    { $$.ast = loop_AST($n0, $n2, 0, 0, $n4); }
  | 'do' unterminated_statement 'while' expression $right 2300
    { $$.ast = loop_AST($n0, $n3, &$n1, 0, $n1); }
  | 'for' '(' expression? ';' expression? ';' expression? ')' unterminated_statement $right 2400
    { $$.ast = loop_AST($n0, $n4, &$n2, &$n6, $n8); }
  ;

binary_operator
  : '.*'        $binary_op_left 9900 
  | '**'        $binary_op_left 9700
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
  | ':='	$binary_op_left 8500
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
  | '[' ']' null $unary_op_left 9850
    [ if (d_ws_before(${parser}, &$n0) != $n0.start_loc.s) ${reject}; ]
    { $0.ast = symbol_AST($g->i, &$n0); }
  ;

object:
    { $$.ast = new_AST(AST_object); }
  ;

null: 
    { $$.ast = new_AST(AST_list); }
  ;

curly_block: '{' statement* expression? '}' { 
  if ($#2)
    $$.ast = new_AST(AST_scope, &$n); 
  else
    $$.ast = new_AST(AST_object, &$n);
  $$.ast->scope_kind = Scope_RECURSIVE;
};

paren_block: '(' statement* expression? ')' { 
  if ($#2)
    $$.ast = new_AST(AST_block, &$n); 
  else
    $$.ast = new_AST(AST_list, &$n);
};

square_block: '[' statement* expression? ']' { 
  if ($#2)
    $$.ast = new_AST(AST_scope, &$n); 
  else
    $$.ast = new_AST(AST_vector, &$n);
  $$.ast->scope_kind = Scope_PARALLEL;
};

constant : (char | int8 | uint8 | int16 | uint16 | 
	    int32 | uint32 | int64 | uint64 | int | uint |
	    float32 | float64 | float128 | float | 
	    complex32 | complex64 | complex128 | complex |
	    string | symbol) ('__name' string)?
{ 
  $$.ast = new_AST(AST_const, &$n); 
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
  $$.ast = new_AST(AST_const); 
  $$.ast->string = if1_cannonicalize_string($g->i, $n0.start_loc.s, $n0.end);
  $$.ast->constant_type = constant_type($n, d_symbols_v);
};

base_float ::= "(([0-9]+.[0-9]*|[0-9]*.[0-9]+)([eE][\-\+]?[0-9]+)?|[0-9]+[eE][\-\+]?[0-9]+)";
float32	   ::= base_float "(f|F)";
float64	   ::= base_float "(d|D)";
float128   ::= base_float "(l|L)";
float	   ::= base_float "/[^\.]";

complex32  ::= float32 "i";
complex64  ::= float64 "i";
complex128 ::= float128 "i";
complex    ::= base_float "i";

identifier : "[a-zA-Z_][a-zA-Z0-9_]*" $term -1;

ident : identifier{ 
  $$.ast = new_AST(AST_ident, &$n); 
  $$.ast->string = if1_cannonicalize_string($g->i, $n0.start_loc.s, $n0.end);
};

 _ : {
  if ($# == 1)
    $$.ast = $0.ast;
};
