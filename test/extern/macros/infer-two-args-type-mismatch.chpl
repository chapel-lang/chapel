use CTypes;

extern proc INFER_ONLY_DOUBLE_FLOAT(x, y): c_int;
writeln(INFER_ONLY_DOUBLE_FLOAT(1:c_double, 2:c_int));
