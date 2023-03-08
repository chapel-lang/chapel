proc foo(param x: int) {
  writeln("Got into param foo with value ", x);
}

proc foo(x: int) {
  writeln("Got into non-param foo with value ", x);
}

proc param int.bar param do return 3;

proc int.bar param do return 4;

proc int.baz do return 5;

foo(77.bar);
foo(77.baz);

config const which = true;

var myVar = if which then -1 else 1;

foo(myVar.bar);
foo(myVar.baz);
