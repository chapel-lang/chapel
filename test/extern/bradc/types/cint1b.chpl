extern type ext_c_int_t;

extern var my_extern_c_int: ext_c_int_t;

extern proc print_c_int(x: ext_c_int_t);

print_c_int(my_extern_c_int);
