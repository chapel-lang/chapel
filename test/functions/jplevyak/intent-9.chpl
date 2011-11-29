

proc foo(out x) {
  x = 7;
}

var y: int;
foo(y);
writeln("expect 7 ", y);
