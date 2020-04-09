module M1 {
  private iter foo(x: int) {
    for i in 1..x {
      yield 10;
    }
  }
}

module M2 {
  use M1 only;
  proc main() {
    writeln(M1.foo(3));
    // Ensures that the private module level iterator foo is not visible
    // via explicit naming.
  }
}
