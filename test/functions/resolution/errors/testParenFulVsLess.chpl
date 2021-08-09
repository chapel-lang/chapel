proc foo {
  writeln("In paren-less foo");
  return [1.0, 2.0];
}

proc foo(x: int) {
  writeln("In paren-ful foo");
  return x + 1;
}

writeln(foo(1));
