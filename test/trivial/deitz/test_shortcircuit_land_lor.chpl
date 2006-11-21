def foo(i: int) {
  if (i / 0 > 2) return true;
  return false;
}

writeln(false && foo(2));
writeln(true || foo(2));
