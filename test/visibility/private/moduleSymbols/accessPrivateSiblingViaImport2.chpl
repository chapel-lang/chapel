module parent {
  private module child {
    proc secretFunction(a: int) {
      return a*3;
    }
  }

  module sibling {
    import parent;
    proc main() {
      writeln(parent.child.secretFunction(11));
    }
  }
}
