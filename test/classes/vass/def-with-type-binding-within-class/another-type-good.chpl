class A {
  int.fooB() { writeln("B.fooB() on ", this); } // on int

  proc mainA() { 5.fooB(); }
}
(new A()).mainA();
6.fooB(); // question: should 'fooB' be visible here?
