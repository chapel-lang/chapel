{
#include "stdio.h"
}

S: A { printf("."); } B;
A: 'a' { printf("a"); };
B: 'b' { printf("b"); };
