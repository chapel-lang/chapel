class B {
  class A { }
  proc A.fooA() { writeln("A.fooA()"); } // on the previously-declared class A

  proc mainB() { (new A()).fooA(); }
}
(new B()).mainB();
(new A()).fooA(); // question: should 'fooA' be visible here?
