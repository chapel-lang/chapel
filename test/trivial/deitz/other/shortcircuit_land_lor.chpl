proc foo(i: int, j: int) {
  if (i / j > 2) then return true;
  return false;
}

writeln(false && foo(2, 0));
writeln(true || foo(2, 0));
