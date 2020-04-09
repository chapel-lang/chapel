

proc foo(inout x) {
  x = x + x;
  return x;
}

var y = 1;
foo(y);
writeln("expect 2 ", y);
