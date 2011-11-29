extern type ext_c_int_t;

var my_c_int: ext_c_int_t;

extern proc set_c_int(inout lhs: ext_c_int_t, rhs: int(32));

extern proc print_c_int(x: ext_c_int_t);

set_c_int(my_c_int, 32);
print_c_int(my_c_int);
