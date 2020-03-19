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
    use this.N only;
    use this.M;
    N.fn();
  }
}
