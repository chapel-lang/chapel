

proc foo(const x) {
  x = 7;
  return x;
}

var y = 1;
foo(y);
writeln("expect 7 ", y);
