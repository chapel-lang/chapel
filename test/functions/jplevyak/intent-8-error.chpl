

proc foo(out x: int) {
  x = x * 2;
  return x;
}

writeln("expect 4 ", foo(2));
