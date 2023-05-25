#include <stdio.h>

static FILE* openTestFile(void) {
    FILE* ret = fopen("./test.txt", "r");
    fclose(ret);
    return ret;
}
