proc foo(x: real(32)) {
  writeln("In foo");
}

proc bar(x: real(64)) {
  writeln("In bar64");
}

proc bar(x: real(32)) {
  writeln("In bar32");
}

proc baz(x: real(32)) {
  writeln("In baz32");
}

proc baz(x: real(64)) {
  writeln("In baz64");
}

proc testit2(type t) {
  var x = 42: t;
  writeln(t:string);
  foo(x);
  bar(x);
  baz(x);
  writeln();
}

proc testit(param size) {
  testit2(int(size));
  testit2(uint(size));
}

testit(32);
testit(16);
testit(8);
