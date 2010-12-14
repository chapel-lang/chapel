class A {
  def A.fooA() { writeln("A.fooA()"); }  // defines fooA() on the enclosing class A
}

var myA = new A();
myA.fooA();
