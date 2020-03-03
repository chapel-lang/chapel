module OuterModule {
  module M {
    var a: int = 42;

    module M {
      var a: int = 23;
    }
  }

  use M;
  use M.M;
  writeln(a);
}
