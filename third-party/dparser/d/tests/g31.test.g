{
#include <stdio.h>
}

${pass sym for_all postorder}
${pass gen for_all postorder}

translation_unit: statement*;
 
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
  sym: { printf("sym =\n"); }
  gen: { printf("gen =\n"); }
  | integer 
  gen: { printf("gen integer\n"); }
  sym: { printf("sym integer\n"); }
  | expression '+' expression $right 2
  sym: { printf("sym +\n"); }
  ;

integer: "-?([0-9]|0(x|X))[0-9]*(u|U|b|B|w|W|L|l)*" $term -1;
identifier: "[a-zA-Z_][a-zA-Z_0-9]*";
