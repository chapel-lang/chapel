function foo(x : string) {
  writeln("string ", x);
}

function foo(x) {
  writeln("any ", x);
}

foo("1");
foo(1);
