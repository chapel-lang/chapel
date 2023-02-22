proc foo(x: bool) {
  writeln("In foo(bool)");
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

proc baz(x: bool) {
  writeln("In baz(bool)");
}

var b: bool;

foo(b);
bar(b);
baz(b);
