proc foo(x: integral = 1..3) {
  writeln(x);
}

foo(1);
writeln();
serial {
  foo();
}
writeln();
serial {
  foo(1..4);
}
