module parent {
  private module child {
    proc secretFunction(a: int) {
      return a*3;
    }
  }

  module sibling {
    import parent.{child}; // use bracketed form instead
    proc main() {
      writeln(child.secretFunction(11));
    }
  }
}
