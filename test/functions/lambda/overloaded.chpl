
proc foo(x : int) {
  writeln("INT: ", x);
}

proc foo(x: string) {
  writeln("STRING: ", x);
}

var x = foo;
