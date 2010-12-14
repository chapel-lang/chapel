class B {
  class A { }
  def A.fooA() { writeln("A.fooA()"); } // on the previously-declared class A

  def mainB() { (new A()).fooA(); }
}
(new B()).mainB();
(new A()).fooA(); // question: should 'fooA' be visible here?
