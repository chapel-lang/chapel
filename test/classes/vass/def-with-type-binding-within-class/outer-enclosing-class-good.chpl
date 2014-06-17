class B {
  class A {
    proc B.fooB() { writeln("B.fooB()"); } // on the outer enclosing class
    proc mainA() { writeln("mainA()"); (new B()).fooB(); }
  }

  proc mainB() { (new A()).mainA(); }
}
(new B()).mainB();
(new B()).fooB(); // question: should 'fooB' be visible here?
