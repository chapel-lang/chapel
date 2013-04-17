proc foo(x: bool(8)) {
  writeln("In foo(bool(8))");
}

proc foo(x: int) {
  writeln("In foo(int)");
}

proc bar(x: int(32)) {
  writeln("In bar(int(32))");
}

proc bar(x: int) {
  writeln("In bar(int)");
}

proc baz(x: bool(8)) {
  writeln("In baz(bool(8))");
}

proc baz(x: bool) {
  writeln("In baz(bool)");
}

var b: bool;

foo(b);
bar(b);
baz(b);

var b8: bool(8);

foo(b8);
bar(b8);
baz(b8);
