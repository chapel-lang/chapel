def foo(i: int) {
  var zz = 0;
  if (i / zz > 2) return true;
  return false;
}

writeln(false && foo(2));
writeln(true || foo(2));
