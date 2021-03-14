module OuterModule {
  module M1 {
    private proc foo(x: int) {
      var res = 10 * x;
      return res;
    }

  }

  writeln(M1.foo(3));
  // Ensures that the private module level variable foo is not visible
  // via explicit naming.
}
