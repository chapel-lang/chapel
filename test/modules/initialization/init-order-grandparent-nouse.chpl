module Grandparent {
  writeln("initializing Grandparent");
  import A;

  module Parent {
    writeln("initializing Parent");
    import B;

    module Child {
      writeln("initializing Child");
      use C, D;
      proc main() { }
    }
  }
}

module A {
  writeln("initializing A");
}

module B {
  writeln("initializing B");
}

module C {
  writeln("initializing C");
}

module D {
  writeln("initializing D");
}
