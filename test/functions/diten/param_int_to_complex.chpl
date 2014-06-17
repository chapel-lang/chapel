proc foo(param c: complex) {
  writeln("param ", c);
}
proc foo(c: complex) {
  writeln("non-param ", c);
}

param param_int: int = 2;
var   var_int  : int = 2;

foo(param_int);
foo(var_int);
