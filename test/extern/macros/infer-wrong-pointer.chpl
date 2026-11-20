use CTypes;

extern proc INFER_ONLY_INT_PTR(p): c_int;
var x: c_double;
writeln(INFER_ONLY_INT_PTR(c_ptrTo(x)));
