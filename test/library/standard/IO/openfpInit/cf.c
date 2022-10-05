#include "cf.h"

FILE* openTestFile(void) {
    return fopen("./test/txt", "r");
}
