module Parent {
  writeln("initializing Parent");
  import A;

  module Child {
    writeln("initializing Child");
    use B, Parent, C;
    proc main() { }
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

