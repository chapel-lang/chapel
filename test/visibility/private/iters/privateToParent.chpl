module M1 {
  private iter foo(x: int) {
    for i in 15..(15+x) {
      yield i;
    }
  }

  module M2 {
    proc main() {
      use M1;
      writeln(foo(3)); // 18
      // Should work, because we are under the parent of the private iterator.
    }
  }
}
