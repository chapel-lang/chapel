proc foo(a, (b, c): (_, int)) {
  writeln("a is: ", a);
  writeln("b is: ", b);
  writeln("c is: ", c);
}


foo(3, (2.1, 4));

