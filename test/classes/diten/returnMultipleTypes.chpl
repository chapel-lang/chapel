class A {
  var name:string = "A";
  def sayTypeName() {
    writeln("A");
  }
}

class B:A {
  var name:string = "B";
  def sayTypeName() {
    writeln("B");
  }
}

class C:B {
  var name:string = "C";
  def sayTypeName() {
    writeln("C");
  }
}

def foo(i: int) {
  if i > 0 then
    return new B();
  else
    return new C();
}

def main {
  var a = foo(3);
  var b = foo(-3);

  a.sayTypeName();
  writeln(a.name);
  b.sayTypeName();
  writeln(b.name);
}
