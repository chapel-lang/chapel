class B {
  class A {
    def B.fooB() { writeln("B.fooB()"); } // on the outer enclosing class
    def mainA() { writeln("mainA()"); (new B()).fooB(); }
  }

  def mainB() { (new A()).mainA(); }
}
(new B()).mainB();
(new B()).fooB(); // question: should 'fooB' be visible here?
