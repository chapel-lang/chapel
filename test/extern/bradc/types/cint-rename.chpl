require "cint.h", "cint.c";
extern "ext_c_int_t" type myCInt;

var my_c_int: myCInt;

extern proc set_c_int(inout lhs: myCInt, rhs: int(32));

extern proc print_c_int(x: myCInt);

set_c_int(my_c_int, 32);
print_c_int(my_c_int);
