module M1 {
  private config const foo = 10;

}

module M2 {
  use M1;

  proc main() {
    writeln(foo);
    // Ensures that the private module level constant foo is not visible
    // via use from a sibling module.
  }
}
