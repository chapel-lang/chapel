module parent {
  private module child {
    proc secretFunction(a: int) {
      return a*3;
    }
  }

  module sibling {
    use parent only child; // use instead of import, and utilize only list
    proc main() {
      writeln(child.secretFunction(11));
    }
  }
}
