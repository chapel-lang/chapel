proc foo(param s: string) {
  writeln("    param: ", s);
}
proc foo(s: string) {
  writeln("non-param: ", s);
}

param int_param: int = 1;
var   int_var:   int = 1;

foo(int_param:string);
foo(int_var:string);
