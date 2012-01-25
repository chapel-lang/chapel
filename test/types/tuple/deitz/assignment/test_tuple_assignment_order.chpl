var x = 1, y = 2;

proc L1() var {
  writeln("L1");
  return x;
}

proc L2() var {
  writeln("L2");
  return y;
}

proc R1() {
  writeln("R1");
  return y;
}

proc R2() {
  writeln("R2");
  return x;
}

(L1(), L2()) = (R1(), R2());

writeln((x, y));
