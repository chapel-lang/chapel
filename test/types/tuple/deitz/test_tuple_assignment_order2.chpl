var x = 1, y = 2;

def L1() var {
  writeln("L1");
  return x;
}

def L2() var {
  writeln("L2");
  return y;
}

def R1() var {
  writeln("R1");
  return y;
}

def R2() var {
  writeln("R2");
  return x;
}

(L1(), L2()) = (R1(), R2());

writeln((x, y));
