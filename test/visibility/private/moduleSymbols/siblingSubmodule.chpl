module parent {
  private module child {
    proc secretFunction(a: int) {
      return a*3;
    }
  }

  module sibling {
    use super.child;

    proc main() {
      writeln(secretFunction(11));
    }

  }
}
