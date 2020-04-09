module OuterModule {
  module M {
    module N {
      proc fn() { writeln("M"); }
    }
  }

  module N {
    proc fn() { writeln("g"); }
  }

  {
    use N only;
    use M;
    N.fn();
  }
}
