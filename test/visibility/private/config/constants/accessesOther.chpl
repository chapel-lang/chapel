module OuterModule {
  module M1 {
    private config const foo = 10;

  }
  import OuterModule.M1;
  writeln(M1.foo);
  // Ensures that the private module level constant foo is not visible
  // via explicit naming.
}
