proc foo(param s: string) {
  writeln("    param: ", s);
}
proc foo(s: string) {
  writeln("non-param: ", s);
}

param real_param: real = 1.0;
var   real_var:   real = 1.0;

foo(real_param:string);
foo(real_var:string);
