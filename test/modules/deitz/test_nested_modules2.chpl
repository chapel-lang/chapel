module OuterModule {
  module M1 {
    var x: int = 2;
  }

  public use M1;

  module M2 {
    use OuterModule;
    proc main {
      writeln(x);
    }
  }
}
