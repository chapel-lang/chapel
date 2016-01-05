module parent {
  private module child {
    proc secretFunction(a: int) {
      return a*3;
    }
  }

  module sibling {
    use child;

    proc main() {
      writeln(secretFunction(11));
    }

  }
}
