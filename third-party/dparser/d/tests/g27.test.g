{
#include <stdio.h>
}
S: A+;
A: 'a' B* { 
  int i; 
  printf("[");
  for (i = 0; i < d_get_number_of_children(&$n1); i++)
    printf("(%c)", *d_get_child(&$n1, i)->start_loc.s);
  printf("]\n");
};
B: 'b' | 'B'; 
