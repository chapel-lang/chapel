module M1 {
  private param foo = 10;

}

module M2 {
  proc main() {
    writeln(M1.foo);
    // Ensures that the private module level param foo is not visible
    // via explicit naming.
  }
}
