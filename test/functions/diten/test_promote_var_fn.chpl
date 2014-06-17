var A: [1..4] int;
proc varFn() var {
  return A;
}

proc promote(i: int): int {
  return i + 1;
}

writeln(promote(varFn()));
