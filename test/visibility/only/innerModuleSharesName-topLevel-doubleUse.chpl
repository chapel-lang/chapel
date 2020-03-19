module OuterModule {
  module M {
    var a: int = 42;

    module M {
      var a: int = 23;
    }
  }

  use this.M;
  use this.M.M;
  writeln(a);
}
