class B {
  proc method() {
    writeln("B.method");
    return 5;
  }
}
class C : B {
  override proc method() {
    writeln("C.method");
    return 100;
  }
}

proc something(in foo : owned B, bar = foo.method()) {
  writeln(bar);
}

{
  var c = new owned C();
  something(c);
}
