{
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "dparse_tables.h"
extern D_Symbol d_symbols_gram[];
char *xdup(char *s, char *e) {
  char *ss = malloc( e - s + 2 );
  memcpy(ss, s, e-s);
  ss[e-s] = 0;
  return ss;
}
}

${pass sym for_all postorder}
${pass gen for_all postorder}

translation_unit: statement*;

_: 
  sym: { 
    printf("default sym(%s : \"%s\")\n",	
	   d_symbols_gram[$n.symbol].name,
	   xdup($n.start_loc.s, $n.end)
	   ); 	
  }
  gen: { 
    printf("default gen(%s : \"%s\")\n",	
	   d_symbols_gram[$n.symbol].name,
	   xdup($n.start_loc.s, $n.end)
	   ); 	
  }
 ;
 
statement 
  : expression ';' 
  { 
    printf("final expression\n"); 
    d_pass(${parser}, &$n, ${pass sym}); 
    d_pass(${parser}, &$n, ${pass gen}); 
  }
  ;

expression 
  : identifier '=' expression $right 1
  sym: { printf("sym expression: =\n"); }
  | integer 
  gen: { printf("gen expression: integer\n"); }
  sym: { printf("sym expression: integer\n"); }
  | expression '+' expression $right 2
  sym: { printf("sym +\n"); }
  ;

integer: "-?([0-9]|0(x|X))[0-9]*(u|U|b|B|w|W|L|l)*" $term -1
  sym: { printf("sym integer\n"); }
  ;
identifier: "[a-zA-Z_][a-zA-Z_0-9]*";
