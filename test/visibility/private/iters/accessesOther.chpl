module OuterModule {
  module M1 {
    private iter foo(x: int) {
      for i in x..(10*x) {
        yield i;
      }
    }

  }
  import OuterModule.M1;
  writeln(M1.foo(3));
  // Ensures that the private module level variable foo is not visible
  // via explicit naming.
}
