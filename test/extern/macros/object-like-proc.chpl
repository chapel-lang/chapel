use CTypes;

extern proc MACRO_SUM_ALIAS(a: c_int, b: c_int): c_int;
writeln(MACRO_SUM_ALIAS(1:c_int, 2:c_int));
