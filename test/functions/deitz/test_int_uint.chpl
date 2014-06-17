proc foo(x: uint(64)) {
  writeln("uint(64) version of foo ", x);
}

proc foo(x: int(64)) {
  writeln("int(64) version of foo ", x);
}

var x: uint(32);
foo(x);
