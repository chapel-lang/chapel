proc foo(x : string) {
  writeln("string ", x);
}

proc foo(x) {
  writeln("any ", x);
}

foo("1");
foo(1);
