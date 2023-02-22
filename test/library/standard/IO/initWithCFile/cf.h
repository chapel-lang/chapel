#include <stdio.h>

static FILE* openTestFile(void) {
    return fopen("./test.txt", "r");
}
