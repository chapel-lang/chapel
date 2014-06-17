proc foo(i: int) {
  writeln("called foo of int with ", i);
}

proc foo(r: real = 2) {
  writeln("called foo of real with ", r);
}

foo(2);
foo(2.0);
foo();
