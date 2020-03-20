module parent {
  private module child {
    proc secretFunction(a: int) {
      return a*3;
    }
  }
  module sibling {
    proc main() {
      import parent.child;
      writeln(child.secretFunction(11));
    }
  }
}
