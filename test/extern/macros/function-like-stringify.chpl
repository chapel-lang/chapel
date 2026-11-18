use CTypes;

extern proc MACRO_STRINGIFY(a: c_int): c_ptrConst(c_char);
writeln(MACRO_STRINGIFY(1:c_int));
