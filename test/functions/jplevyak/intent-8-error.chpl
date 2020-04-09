

proc foo(out x) {
  x = x * 2;
  return x;
}

writeln("expect 4 ", foo(2));
