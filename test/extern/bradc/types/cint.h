#ifndef cint_h
#define cint_h
#include <stdint.h>

typedef int ext_c_int_t;
typedef int32_t c_int32;

extern ext_c_int_t my_extern_c_int;
extern int32_t my_extern_c_int32;

void print_c_int(int x);

void set_c_int(ext_c_int_t* lhs, int32_t rhs);

#endif
