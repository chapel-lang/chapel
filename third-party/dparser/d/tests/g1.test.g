{
#include <stdio.h>
}
S: A S 'b' | 'x';
A: [ printf("speculative e-reduce A\n"); ] 
   { printf("final e-reduce A\n"); };
