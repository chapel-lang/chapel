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
    use this.M;
    use this.N only;
    N.fn();
  }
}
