  module GenericLibraryZ {
    // uses foo()
    proc callFoo(param x) {
      foo();
    }
  }
  module LibraryXA {
    use GenericLibraryZ;
    proc foo() {
      writeln("In A.foo()");
    }
    proc runXA() {
      callFoo(1);
    }
  }
  module LibraryYA {
    use GenericLibraryZ;
    proc foo() {
      writeln("In B.foo()");
    }
    proc runYA() {
      callFoo(1);
    }
  }
  module ApplicationB {
    use LibraryXA;
    use LibraryYA;
    proc main() {
      runXA();
      runYA();
    }
  }
