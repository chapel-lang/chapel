use CTypes;

extern proc INFER_ONLY_FLOAT(x): c_int;
writeln(INFER_ONLY_FLOAT(1:c_int));
