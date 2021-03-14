
class C {
  proc type myTypeMethod() { writeln("C.myTypeMethod()"); }
}
class D: C {
  override proc type myTypeMethod() { writeln("D.myTypeMethod()"); }
}

C.myTypeMethod();
D.myTypeMethod();
