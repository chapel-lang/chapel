def foo(param s: string) {
  writeln("    param: ", s);
}
def foo(s: string) {
  writeln("non-param: ", s);
}

param int_param: int = 1;
var   int_var:   int = 1;

foo(int_param);
foo(int_var);
