{
#include <stdio.h>
}

S: identifier*
  { printf("identifier\n"); }
 ;

identifier: "[a-z<>=+]+";
