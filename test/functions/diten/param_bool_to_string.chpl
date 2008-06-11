def foo(param s: string) {
  writeln("    param: ", s);
}
def foo(s: string) {
  writeln("non-param: ", s);
}

param bool_param: bool = true;
var   bool_var:   bool = true;

foo(bool_param);
foo(bool_var);
