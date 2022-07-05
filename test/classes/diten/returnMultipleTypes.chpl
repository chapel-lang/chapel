class A {
  proc name:string { return "A"; }
  proc sayTypeName() {
    writeln("A");
  }
}

class B:A {
  override proc name:string { return "B"; }
  override proc sayTypeName() {
    writeln("B");
  }
}

class C:B {
  override proc name:string { return "C"; }
  override proc sayTypeName() {
    writeln("C");
  }
}

proc foo(i: int) {
  if i > 0 then
    return new owned B();
  else
    return new owned C();
}

proc main {
  var a = foo(3);
  var b = foo(-3);

  a.sayTypeName();
  writeln(a.name);
  b.sayTypeName();
  writeln(b.name);
}
