_extern type c_int;

_extern var my_extern_c_int: c_int;

_extern proc print_c_int(x: c_int);

print_c_int(my_extern_c_int);
