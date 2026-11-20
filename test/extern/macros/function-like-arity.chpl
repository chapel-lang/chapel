use CTypes;

extern proc MACRO_SUM(a: c_int): c_int;
writeln(MACRO_SUM(1:c_int));
