module M1 {
  private iter foo(x: int) {
    for i in 1..x {
      yield i;
    }
  }

}

module M2 {
  use M1;

  proc main() {
    writeln(foo(6));
    // Ensures that the private module level iterator foo is not visible
    // via use from a sibling module.
  }
}
