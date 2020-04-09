proc foo(x : int = 4) {
  writeln("foo of ", x);
  bar(x);
  bar();
}

proc bar(x : real = 2.0) {
  writeln("bar of ", x);
}

foo();
