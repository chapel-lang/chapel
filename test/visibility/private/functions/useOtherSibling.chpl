module M1 {
  private proc foo(x: int) {
    var res = 10 + x;
    return res;
  }

}

module M2 {
  use M1;

  proc main() {
    writeln(foo(6));
    // Ensures that the private module level function foo is not visible
    // via use from a sibling module.
  }
}
