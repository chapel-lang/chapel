proc foo(i : int) {
  writeln("In int version: ", i);
}

proc foo(param i: int) {
  writeln("In param version: ", i);
}

var x = 13;

foo(12);
foo(x);
