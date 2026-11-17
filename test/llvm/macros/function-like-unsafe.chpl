use CTypes;

extern proc MACRO_MUTATE(a: c_int): c_int;
writeln(MACRO_MUTATE(1:c_int));
