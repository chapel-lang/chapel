module parent {
  private module child {
    var secretVariable = 33;
  }

  use child;
  // Valid, because this module is the direct parent of the private module

  module sibling {
    use parent;
    proc main() {
      writeln(child.secretVariable);
    }
  }
}
