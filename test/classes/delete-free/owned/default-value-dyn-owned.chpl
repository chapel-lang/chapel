class B {
  proc method() {
    writeln("B.method");
    return 5;
  }
}
class C : B {
  proc method() {
    writeln("C.method");
    return 100;
  }
}

proc something(foo : owned B, bar = foo.method()) {
  writeln(bar);
}

var c = new owned C();
something(c);
