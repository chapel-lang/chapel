proc foo(i: int) {
  writeln(i);
  return i;
}

writeln([i in 1..4] foo(i));
