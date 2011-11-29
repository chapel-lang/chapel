var x, y: int;

proc foo() var {
  return x + y;
}

foo() = 2;

writeln(x, y);
