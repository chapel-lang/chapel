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
    proc testA() {
      callFoo(1);
    }
  }
  module ApplicationA {
    use LibraryXA;
    proc main() {
      testA();
    }
  }
