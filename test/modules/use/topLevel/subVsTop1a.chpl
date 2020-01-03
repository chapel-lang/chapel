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
    use M;
    use N only;
    N.fn();
  }
}
