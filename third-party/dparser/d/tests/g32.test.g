{
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
char *xdup(char *s, char *e) {
  char *ss = malloc( e - s + 2 );
  memcpy(ss, s, e-s);
  ss[e-s] = 0;
  return ss;
}
}

translation_unit: statement*;
 
statement 
  : expression ';' 
  { printf("(%s)expression ;(%s)", 
           xdup(d_ws_before(${parser}, &$n), $n.start_loc.s),
           xdup($n.end, d_ws_after(${parser}, &$n)));
  }
  ;

expression 
  : identifier '=' expression $right 1
  { printf("(%s)= in expression(%s)", 
           xdup(d_ws_before(${parser}, &$n1), $n1.start_loc.s),
           xdup($n1.end, d_ws_after(${parser}, &$n1))); 
  }
  | integer 
  { printf("(%s)expression: integer(%s)", 
           xdup(d_ws_before(${parser}, &$n0), $n0.start_loc.s),
           xdup($n0.end, d_ws_after(${parser}, &$n0))); 
  }
  | expression '+' expression $right 2
  { printf("(%s)+ in expression(%s)", 
           xdup(d_ws_before(${parser}, &$n1), $n1.start_loc.s),
           xdup($n1.end, d_ws_after(${parser}, &$n1))); 
  }
  ;

integer: "-?([0-9]|0(x|X))[0-9]*(u|U|b|B|w|W|L|l)*" $term -1;
identifier: "[a-zA-Z_][a-zA-Z_0-9]*"
  { printf("(%s)identifier(%s)", 
           xdup(d_ws_before(${parser}, &$n0), $n0.start_loc.s),
           xdup($n0.end, d_ws_after(${parser}, &$n0))); 
  }
  ;
