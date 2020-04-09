module parent {
  private module child {
    proc secretFunction(a: int) {
      return a*3;
    }
  }

  use child;
  // Valid, because this module is the direct parent of the private module

  module sibling {
    use parent;
    proc main() {
      writeln(child.secretFunction(11));
    }
  }
}
