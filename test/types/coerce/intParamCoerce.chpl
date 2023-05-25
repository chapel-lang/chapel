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

proc goo(x: complex(64)) {
  writeln("In goo64");
}

proc goo(x: complex(128)) {
  writeln("In goo128");
}

proc boo(x: complex(128)) {
  writeln("In boo128");
}

proc boo(x: complex(64)) {
  writeln("In boo64");
}

proc testit2(type t) {
  param x = 42: t;
  writeln(t:string);
  foo(x);
  bar(x);
  baz(x);
  goo(x);
  boo(x);
  writeln();
}

proc testit(param size) {
  testit2(int(size));
  testit2(uint(size));
}

testit(64);
testit(32);
testit(16);
testit(8);
