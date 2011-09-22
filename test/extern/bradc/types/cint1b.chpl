extern type c_int;

extern var my_extern_c_int: c_int;

extern proc print_c_int(x: c_int);

print_c_int(my_extern_c_int);
