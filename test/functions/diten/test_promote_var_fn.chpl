var A: [1..4] int;
def varFn() var {
  return A;
}

def promote(i: int): int {
  return i + 1;
}

writeln(promote(varFn()));
