use CTypes;

extern proc ONLY_INT(x: c_double): c_int;
writeln(ONLY_INT(2.9));
