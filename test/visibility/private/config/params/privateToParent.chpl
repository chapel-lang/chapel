module M1 {
  private config param foo = 15;

  module M2 {
    proc main() {
      writeln(foo);
      // Should work, because we are under the parent of the private param.
    }
  }
}
