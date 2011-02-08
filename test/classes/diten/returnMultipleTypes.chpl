class A {
  var name:string = "A";
  proc sayTypeName() {
    writeln("A");
  }
}

class B:A {
  var name:string = "B";
  proc sayTypeName() {
    writeln("B");
  }
}

class C:B {
  var name:string = "C";
  proc sayTypeName() {
    writeln("C");
  }
}

proc foo(i: int) {
  if i > 0 then
    return new B();
  else
    return new C();
}

proc main {
  var a = foo(3);
  var b = foo(-3);

  a.sayTypeName();
  writeln(a.name);
  b.sayTypeName();
  writeln(b.name);

  delete a;
  delete b;
}
