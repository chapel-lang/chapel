

proc foo(in x) {
  x = 7;
  return x;
}

var y = 1;
foo(y);
writeln("expect 1 ", y);
