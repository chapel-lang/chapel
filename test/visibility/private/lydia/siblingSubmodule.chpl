module parent {
  private module child {
    proc secretFunction(a: int) {
      return a*3;
    }
  }

  module sibling {
    use child; // Should this be an error on 'child' or 'parent.child'?

    proc main() {
      writeln(secretFunction(11));
    }

  }
}
