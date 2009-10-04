def foo(x: integral = 1..3) {
  writeln(x);
}

foo(1);
writeln();
foo();
writeln();
serial true {
  foo(1..4);
}
