{
#include <stdio.h>
}
S : 'a' ('b' { printf("(b)\n"); })* 'c';
