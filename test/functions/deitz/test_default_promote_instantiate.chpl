proc foo(x: integral = 1..3) {
  writeln(x);
}

foo(1);
writeln();
serial true {
  foo();
}
writeln();
serial true {
  foo(1..4);
}
