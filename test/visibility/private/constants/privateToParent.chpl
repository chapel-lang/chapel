module M1 {
  private const foo = 15;

  module M2 {
    proc main() {
      use M1;
      writeln(foo);
      // Should work, because we are under the parent of the private constant.
    }
  }
}
