var x, y: int;

proc foo() ref {
  return x + y;
}

foo() = 2;

writeln(x, y);
