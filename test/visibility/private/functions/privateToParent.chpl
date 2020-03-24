module M1 {
  private proc foo(x: int) {
    var res = 15 + x;
    return res;
  }

  module M2 {
    proc main() {
      import M1.foo;
      writeln(foo(3)); // 18
      // Should work, because we are under the parent of the private function.
    }
  }
}
