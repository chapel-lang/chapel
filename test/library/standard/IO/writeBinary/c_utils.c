
#include "c_utils.h"

unsigned char* point_to_char_array(void) {
    unsigned char arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    unsigned char *ptr = arr;

    return ptr;
}

unsigned short* point_to_short_array(void) {
    unsigned short arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    unsigned short *ptr = arr;

    return ptr;
}

void* point_to_void_char_array(void) {
    unsigned char arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    void *ptr = arr;

    return ptr;
}

void* point_to_void_short_array(void) {
    unsigned short arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    void *ptr = arr;

    return ptr;
}
