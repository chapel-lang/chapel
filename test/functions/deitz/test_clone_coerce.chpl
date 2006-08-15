def foo(x : string) {
  writeln("string ", x);
}

def foo(x) {
  writeln("any ", x);
}

foo("1");
foo(1);
