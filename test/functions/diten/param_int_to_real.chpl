proc foo(param r: real) {
  writeln("param ", r);
}
proc foo(r: real) {
  writeln("non-param ", r);
}

param param_int: int = 2;
var   var_int  : int = 2;

foo(param_int);
foo(var_int);
