proc foo(param x: uint(8)) where x == 0 {
  writeln("In foo()");
}

foo(1.2);
