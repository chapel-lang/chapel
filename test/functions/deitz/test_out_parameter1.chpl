proc foo(out x : int) {
  writeln("expect 0, get ", x);
  x = 7;
  writeln("expect 7, get ", x);
}

var y : int = 2;
writeln("expect 2, get ", y);
foo(y);
writeln("expect 7, get ", y);
