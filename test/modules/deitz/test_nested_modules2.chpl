module OuterModule {
  module M1 {
    var x: int = 2;
  }

  use this.M1;

  module M2 {
    proc main {
      writeln(x);
    }
  }
}
