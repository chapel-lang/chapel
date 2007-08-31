#include <stdio.h>

char * c_realStr(char * buf, char * fmt, double x) {
    sprintf(buf, fmt, x);
    return buf;
}

char * c_intStr(char * buf, char * fmt, double x) {
    sprintf(buf, fmt, x);
    return buf;
}
