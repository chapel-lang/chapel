/*
 Sample Grammar
*/

{
#include "d.h"
}

${declare longest_match program}

program : statement*;

statement : external | definition | expression ';' ;

definition : identifier ':' expression ';' 
{
  D_Sym *s;
  char *ts = dup_str($n0.start_loc.s, $n0.end);
  ${scope} = new_D_Scope(${scope});
  s = NEW_D_SYM(${scope}, $n0.start_loc.s, $n0.end); 
  printf("def Sym '%s' line %d: %X\n", ts,  $n0.start_loc.line, (int)(uintptr_t)s);
  d_free(ts);
  $$ = s;
};

external
	: 'extern' external_type identifier ';'
	| 'extern' external_type identifier 
	'(' (external_type (',' external_type)*)? ')' ';'
	;

external_type : builtin_types;

expression
	: constant
	| identifier {
	  D_Sym *s = find_D_Sym(${scope}, $n0.start_loc.s, $n0.end);
          char *ts = dup_str($n0.start_loc.s, $n0.end);
	  if (s)
  	    printf("ref Sym '%s' line %d val %d: %X of %X\n",
	           ts, $n0.start_loc.line,
		   s->user, (int)(uintptr_t)s, (int)(uintptr_t)s->update_of);
	  else
 	    printf("ref Sym '%s' line %d: not found\n",
                   ts, $n0.start_loc.line);
          d_free(ts);
	}	
	| pre_operator expression
	| expression post_operator
	| expression binary_operator expression
	| '{' expression '}'
	| '(' expression ')'
	| '[' expression ']'
	| expression '?' expression ':' expression $right 8600
	| 'if' '(' expression ')' expression $right 6000
	| 'if' '(' expression ')' expression 'else' expression $right 6100
	| 'while' '(' expression ')' expression $right 6200
	| 'do' expression 'while' expression $right 6300
	| 'for' '(' expression (';' expression (';' expression)?)? ')'
	   expression $right 6400
	;

constant : integer | float | character | strings;

strings : string+;

binary_operator
	: '.'		$binary_op_left 9900
	| '*'		$binary_op_left 9600
	| '/'		$binary_op_left 9600
	| '%'		$binary_op_left 9600
	| '+'		$binary_op_left 9500
	| '-'		$binary_op_left 9500
	| '<<'		$binary_op_left 9400
	| '>>'		$binary_op_left 9400
	| '<'		$binary_op_left 9300
	| '<='		$binary_op_left 9300
	| '>'		$binary_op_left 9300
	| '>='		$binary_op_left 9300
	| '=='		$binary_op_left 9200
	| '!='		$binary_op_left 9200
	| '&'		$binary_op_left 9100
	| '^'		$binary_op_left 9000
	| '|'		$binary_op_left 8900
	| '&&'		$binary_op_left 8800
	| '||'		$binary_op_left 8700
	| '='		$binary_op_left 8500
	| '*='		$binary_op_left 8500
	| '/='		$binary_op_left 8500
	| '%='		$binary_op_left 8500
	| '+='		$binary_op_left 8500
	| '-='		$binary_op_left 8500
	| '<<='		$binary_op_left 8500
	| '>>='		$binary_op_left 8500
	| '&='		$binary_op_left 8500
	| '|='		$binary_op_left 8500
	| '^='		$binary_op_left 8500
	| ','		$binary_op_left 8400
	| '->'		$binary_op_left 9900
	| '::'		$binary_op_right 10000
	|		$binary_op_left 7000
	;

pre_operator
	: '-'		$unary_op_right 9800
	| '+'		$unary_op_right 9800
	| '~'		$unary_op_right 9800
	| '!'		$unary_op_right 9800
	| '*'		$unary_op_right 9800
	| '&'		$unary_op_right 9800
	| '--'		$unary_op_right 9800
	| '++'		$unary_op_right 9800
	| '(' external_type ')' $unary_op_right 9800
	| 'sizeof'	 $unary_op_right	9900
	| '::'		 $unary_op_right	10000
	;

post_operator
	: '--' 	$unary_op_left 9800
	| '++' 	$unary_op_left 9800
	| '{' expression '}' $unary_op_left 9900
	| '(' expression ')' $unary_op_left 9900
	| '[' expression ']' $unary_op_left 9900
	;

builtin_types
	: 'int'
	| 'uint'
	| 'int8'
	| 'uint8'
	| 'int16'
	| 'uint16'
	| 'int32'
	| 'uint32'
	| 'int64'
	| 'uint64'
	| 'float'
	| 'float32'
	| 'float64'
	;

character: "'[^']*'";
string: "\"[^\"]*\"";
integer: "-?([0-9]|0(x|X))[0-9]*(u|U|b|B|w|W|L|l)*" $term -1;
float: "[\-+]?([0-9]+\.[0-9]*|\.[0-9]+)([eE][\-+]?[0-9]+)?" $term -2;

{ 
  char *reserved_words[] = { "if", "else", "extern", "sizeof", "int", "uint",
    "int8", "uint8", "int16", "uint16", "int32", "uint32", "int64", "uint64",
    "float", "float32", "float64", NULL };

  static int is_one_of(char *s, char **list) {
    while (*list) { if (!strcmp(s, *list)) return 1; list++; }
    return 0;
  }
}
identifier: "[a-zA-Z_][a-zA-Z_0-9]*" $term -3 [ 
  char *ts = dup_str($n0.start_loc.s, $n0.end);
  if (is_one_of(ts, reserved_words)) {
    d_free(ts);
    ${reject};
  }
  d_free(ts);
];



