

proc foo(inout x) {
  x = 7;
  return x;
}

foo(7);
writeln("expect 7 ", y);
