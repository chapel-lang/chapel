module OuterModule {
  module M1 {
    private config param foo = 10;

  }
  import OuterModule.M1;
  writeln(M1.foo);
  // Ensures that the private module level param foo is not visible
  // via explicit naming.
}
