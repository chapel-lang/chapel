#include <stdio.h>
#include "adder.h"
int main() {
    int a = 1;
    int b = 2;
    int c = add(a, b);
    printf("a + b = %d\n", c);
}
