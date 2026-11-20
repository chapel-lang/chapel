use CTypes;

extern proc MACRO_ADDR(a: c_int): c_ptr(c_int);
writeln(MACRO_ADDR(1:c_int));
