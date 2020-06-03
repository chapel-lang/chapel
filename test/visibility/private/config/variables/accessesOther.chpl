module OuterModule {
    module M1 {
    private config var foo = 10;

  }

  writeln(M1.foo);
  // Ensures that the private module level variable foo is not visible
  // via explicit naming.
}
