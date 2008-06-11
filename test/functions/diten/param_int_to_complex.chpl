def foo(param c: complex) {
  writeln("param ", c);
}
def foo(c: complex) {
  writeln("non-param ", c);
}

param param_int: int = 2;
var   var_int  : int = 2;

foo(param_int);
foo(var_int);
