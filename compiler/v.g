/*
  Copyright 2003 John Plevyak, All Rights Reserved
*/

%<
#include "geysa.h"
%>

%<

#define internal_D_Sym(_x) \
  char *_x##_start = #_x; \
  char *_x##_end = _x##_start + sizeof(#_x) - 1

internal_D_Sym(__module);

#define find_internal(_s, _x) find_D_Sym(_s, _x##_start, _x##_end)
#define new_internal(_s, _x) NEW_D_SYM(_s, _x##_start, _x##_end)

extern D_ParserTables parser_tables_v;
extern D_Symbol d_symbols_v[];

static char *
constant_type(D_ParseNode &pn) {
  D_ParseNode *child = d_get_child(&pn, 0);
  while (d_symbols_v[child->symbol].kind != D_SYMBOL_REGEX && 
	 d_symbols_v[child->symbol].kind != D_SYMBOL_NTERM)
    child = d_get_child(child, 0);
  return d_symbols_v[child->symbol].name;
}

D_Sym *
new_sym(Globals *g, char *s, char *e, D_Scope **scope, int scope_kind = D_SCOPE_INHERIT) {
  D_Sym *sym = NEW_D_SYM(*scope, s, e);
  if (scope_kind == D_SCOPE_INHERIT)
    scope_kind = (*scope)->kind;
  if (scope_kind == D_SCOPE_SEQUENTIAL)
    *scope = enter_D_Scope(*scope, *scope);
  return sym;
}

void
in_module(Globals *g, char *s, char *e, D_Scope **scope) {
  D_Scope *global = global_D_Scope(*scope);
  D_Sym *sym = find_D_Sym(global, s, e);
  if (!sym)
    sym = NEW_D_SYM(global, s, e);
  if (sym->user.scope)
    *scope = enter_D_Scope(*scope, sym->user.scope);
  else {
    *scope = sym->user.scope = new_D_Scope(global);
    D_Sym *s = new_internal(*scope, __module);
    s->user.sym = sym;
  }
}

AST *
loop_AST(D_ParseNode &loop, D_ParseNode &cond, D_ParseNode *before, D_ParseNode *after, D_ParseNode &body) {
  AST *b = body.user.ast;
  if (after) {
    b = new AST(AST_block);
    b->set_location(&body);
    b->add(body.user.ast);
    b->add(after->user.ast);
  }
  AST *a = new AST(AST_loop);
  a->set_location(&loop);
  if (before && before != &body)
    a->add(before->user.ast);
  if (before != &body)
    a->add(new AST(AST_loop_cond, &cond));
  a->add(b);
  if (before == &body)
    a->add(new AST(AST_loop_cond, &cond));
  return NULL;
}

AST *
symbol_AST(IF1 *if1, D_ParseNode *pn) {
  AST *a = new AST(AST_const, pn); 
  char *s;
  int l = pn->end - pn->start_loc.s;
  if (!l)
    s = "#^^";
  else {
    s =  (char*)MALLOC(l + 1);
    memcpy(s+1, pn->start_loc.s, l);
    s[0] = '#';
  }
  a->string = if1_cannonicalize_string(if1, s);
  a->constant_type = "symbol";
  return a;
}

AST *
op_AST(IF1 *if1, D_ParseNode &pn) {
  AST *op = new AST(AST_op);
  op->set_location(&pn);
  for (int i = 0; i < d_get_number_of_children(&pn); i++) {
    D_ParseNode *c = d_get_child(&pn, i);
    if (c->user.ast)
      op->add(c->user.ast);
    else {
      op->op_index = i;
      op->add(symbol_AST(if1, c));
    }
  }
  return op;
}

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
  : 'in' identifier ('__name' string)? ';' 
[ in_module($g, $n1.start_loc.s, $n1.end, &${scope}); ]
{ 
  $$.ast = new AST(AST_in_module, &$n); 
  if ($#2)
    $$.ast->builtin = if1_cannonicalize_string(
      $g->i, ${child 2, 0, 1}->start_loc.s+1, ${child 2, 0, 1}->end-1);
}
  | 'use' identifier ';'
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
  : module_identifier 
  | module_identifier 'as' module_identifier
{ $$.ast = new AST(AST_as, &$n); };
module_identifier: qualified_identifier ('.' identifier)*;

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
  | identifier ':' ';'
{ $$.ast = new AST(AST_label, &$n); }
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

pure_type_statement 
  : some_pure_type_statement ';'
{ $$.ast = $0.ast; }
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
  | 'class' def_type def_type_parameter_list? class_definition? 
{ $$.ast = new AST(AST_def_type, &$n); }
  | 'enum' def_type enum_definition? 
{ $$.ast = new AST(AST_def_type, &$n); }
  ;

unqualified_pure_type_statement
  : def_identifier type
{  $$.ast = new AST(AST_declare_ident, &$n); }
  | unqualified_type_statement
  ;

type_definition : def_type def_type_parameter_list? ('__name' string)? (':' super_type)? ('=' type)? { 
  $$.ast = new AST(AST_def_type, &$n); 
  if ($#2)
    $$.ast->builtin = if1_cannonicalize_string(
      $g->i, ${child 2, 0, 1}->start_loc.s+1, ${child 2, 0, 1}->end-1);
};
super_types: super_type (',' super_type)*;
super_type: parameterized_type
{ $$.ast = new AST(AST_super_type, &$n); };

def_type: identifier 
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
  | identifier 'of' type	$binary_right 300
{ $$.ast = new AST(AST_tagged_type, &$n); }
  | type type_parameter_list	$unary_left 200
{ $$.ast = new AST(AST_type_application, &$n); }
  | '(' type  ')'
  | class_definition
{ $$.ast = new AST(AST_record_type, &$n); }
  | qualified_identifier
  | constant
  ;

type_parameter_list : '(' type_param (',' type_param)* ')' ;
type_param: type { $$.ast = new AST(AST_type_param, &$n); };

class_definition : class_modifiers '{' new_class_scope pure_type_statement* '}' [
  $$.saved_scope = ${scope};
  ${scope} = enter_D_Scope(${scope}, $n0.scope);
];
new_class_scope: [ ${scope} = new_D_Scope(${scope}); ${scope}->kind = D_SCOPE_RECURSIVE; ];

class_modifiers : (class_modifier (',' class_modifiers)*)?;
class_modifier  
  : (':'  | 'inherits') inherits_identifier (',' inherits_identifier)*
  | (':>' | 'implements') implements_identifier (',' implements_identifier)*
  | (':+' | 'includes') includes_identifier (',' includes_identifier)*
  ;
inherits_identifier: parameterized_type { $$.ast = new AST(AST_super_type, &$n); };
implements_identifier: parameterized_type { $$.ast = new AST(AST_implements, &$n); };
includes_identifier: parameterized_type { $$.ast = new AST(AST_includes, &$n); };
parameterized_type
  : qualified_identifier 
  | qualified_identifier type_parameter_list
{ $$.ast = new AST(AST_type_application, &$n); };

enum_definition : '{' enumerator (',' enumerator)* '}';
enumerator: identifier ('=' expression)? 
{ $$.ast = new AST(AST_tagged_type, &$n); };

vector_type : '[' (vector_index (',' vector_index)*)? ']';
vector_index 
  : anyint 
{ $$.ast = new AST(AST_index, &$n); }
  | anyint '..' anyint
{ $$.ast = new AST(AST_index, &$n); }
  ;

where_statement : 'where' where_type (',' where_type)*;
where_type : qualified_identifier (':' super_types)? ('=' type)?
{ $$.ast = new AST(AST_where, &$n); };

expression
  : constant ("__name" string)?
   { 
     $$.ast = $0.ast; 
     if ($#1)
       $$.ast->builtin = if1_cannonicalize_string(
         $g->i, ${child 1, 0, 1}->start_loc.s+1, ${child 1, 0, 1}->end-1);
   }
  | qualified_identifier
  | def_identifier expression $right 5100
    { 
      $$.ast = new AST(AST_def_ident, &$n); 
      $$.ast->def_ident_label = 1;
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
  | expression ('.' $name "op period" | '->' $name "op arrow") symbol_identifier $left 9900
    { $$.ast = op_AST($g->i, $n); }
  | curly_block
  | paren_block
  | curly_block
  | expression '?' expression ':' expression $right 8600
    { $$.ast = new AST(AST_if, &$n); }
  | 'if' '(' expression ')' expression $right 6000
    { $$.ast = new AST(AST_if, &$n); }
  | 'if' '(' expression ')' expression 'else' expression $right 6100
    { $$.ast = new AST(AST_if, &$n); }
  | 'while' loop_scope '(' expression ')' expression $right 6200
    { $$.ast = loop_AST($n0, $n3, 0, 0, $n5); }
  | 'do' loop_scope expression 'while' expression $right 6300
    { $$.ast = loop_AST($n0, $n4, &$n2, 0, $n2); }
  | 'for' loop_scope '(' expression? ';' expression? ';' expression? ')' expression
    { $$.ast = loop_AST($n0, $n3, &$n5, &$n7, $n9); }
  | 'with' with_scope expression $right 5100
    [ ${scope} = enter_D_Scope(${scope}, $n0.scope); ]
    { $$.ast = new AST(AST_with, &$n); }
  | 'conc' expression $right 5000
    { $$.ast = new AST(AST_conc, &$n); }
  | 'seq' expression $right 5000
    { $$.ast = new AST(AST_seq, &$n); }
  ;

loop_scope: [ ${scope} = new_D_Scope(${scope}); ]; 
with_scope : expression (',' expression)* ':'
[ ${scope} = new_D_Scope(${scope}); ] 
{ $$.ast = new AST(AST_with_scope, &$n); };

symbol_identifier: identifier
{ $$.ast = symbol_AST($g->i, &$n); };

def_identifier: identifier ('__name' string)? ':' 
{
  $$.ast = $0.ast;
  if ($#1)
    $$.ast->builtin = if1_cannonicalize_string(
      $g->i, ${child 1, 0, 1}->start_loc.s+1, ${child 1, 0, 1}->end-1);
};

def_function: qualified_identifier pattern+ ('__name' string)? ':' 
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

anon_function: '\\' pattern+ ('__name' string)? ':' 
[ 
  $$.saved_scope = ${scope}; 
  ${scope} = new_D_Scope(${scope}); 
];

pattern
  : identifier (':' type)?
{ $$.ast = new AST(AST_arg, &$n); }
  | constant
{ $$.ast = new AST(AST_arg, &$n); }
  | '(' (sub_pattern (',' sub_pattern)*)? ')'
{ $$.ast = new AST(AST_pattern, &$n); };
  ;

sub_pattern
  : pattern
  | '...' (identifier (':' type)?)?
{ $$.ast = new AST(AST_vararg, &$n); };
  ;

qualified_identifier : global? (identifier '::')* identifier
{ $$.ast = new AST(AST_qualified_ident, &$n); };

global: '::' 
{ $$.ast = new AST(AST_global, &$n); } ;

with_scope : expression (',' expression)* ':' ;

control_flow
  : 'goto' identifier 
{ $$.ast = new AST(AST_goto, &$n); }
  | 'continue' identifier? 
{ $$.ast = new AST(AST_continue, &$n); }
  | 'break' identifier? 
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
  | ',' 	$binary_op_left 8400 
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

constant : (character | int8 | uint8 | int16 | uint16 | 
	    int32 | uint32 | int64 | uint64 | int | uint |
	    float32 | float64 | float128 | float | 
	    string | symbol | complex) ('__name' string)?
{ 
  $$.ast = new AST(AST_const, &$n); 
  $$.ast->string = if1_cannonicalize_string($g->i, $n0.start_loc.s, $n0.end);
  $$.ast->constant_type = constant_type($n);
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
character ::= $name "character" "\'([^\'\\]|\\[^])*\'";

base_int ::= "(0[0-7]*|(0x|0X)[0-9a-fA-F]+|[1-9][0-9]*)";
int8	 ::= $name "int8" base_int "(b|B)";
uint8	 ::= $name "uint8" base_int "((u|U)(b|B)|(b|B)(u|U)";
int16	 ::= $name "int16" base_int "(s|S)";
uint16	 ::= $name "uint16" base_int "(u|U)(s|S)|(s|S)(u|U)";
int32	 ::= $name "int32" base_int "(w|W)";
uint32	 ::= $name "uint32" base_int "(u|U)(w|W)|(w|W)(u|U)";
int64	 ::= $name "int64" base_int "(l|L)";
uint64	 ::= $name "uint64" base_int "(u|U)(l|L)|(u|U)(l|L)";
int	 ::= $name "int" base_int;
uint	 ::= $name "uint" base_int "(u|U)";
anyint	 : character | int8 | uint8 | int16 | uint16 | 
           int32 | uint32 | int64 | uint64 | int | uint
{ 
  $$.ast = new AST(AST_const); 
  $$.ast->string = if1_cannonicalize_string($g->i, $n0.start_loc.s, $n0.end);
  $$.ast->constant_type = constant_type($n);
};

base_float ::= "(([0-9]+.[0-9]*|[0-9]*.[0-9]+)([eE][\-\+]?[0-9]+)?|[0-9]+[eE][\-\+]?[0-9]+)";
float32	   ::= $name "float32" base_float "(f|F)";
float64	   ::= $name "float64" base_float "(d|D)";
float80	   ::= $name "float80" base_float "(t|T)";
float128   ::= $name "float128" base_float "(l|L)?";
float	   ::= $name "float" base_float;
anyfloat   : float32 | float64 | float128 | float;

anynum ::= character | int8 | uint8 | int16 | uint16 | 
           int32 | uint32 | int64 | uint64 | int | uint |
           float32 | float64 | float128 | float;

complex	   ::= $name "complex" anynum "i" ;

identifier : "[a-zA-Z_][a-zA-Z0-9_]*" $term -1 { 
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
