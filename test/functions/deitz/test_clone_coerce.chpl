fun foo(x : string) {
  writeln("string ", x);
}

fun foo(x) {
  writeln("any ", x);
}

foo("1");
foo(1);
