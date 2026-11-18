use CTypes;

extern proc MACRO_VARIADIC(a: c_int): c_int;
writeln(MACRO_VARIADIC(1:c_int));
