proc foo(param s: string) {
  writeln("    param: ", s);
}
proc foo(s: string) {
  writeln("non-param: ", s);
}

param bool_param: bool = true;
var   bool_var:   bool = true;

foo(bool_param:string);
foo(bool_var:string);
