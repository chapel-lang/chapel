module M1 {
  private proc foo(x: int) {
    var res = 10 + x;
    return res;
  }
}

module M2 {
  use M1 only;
  proc main() {
    writeln(M1.foo(3));
    // Ensures that the private module level function foo is not visible
    // via explicit naming.
  }
}
