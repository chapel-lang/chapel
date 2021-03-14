module parent {
  private module child {
    proc secretFunction(a: int) {
      return a*3;
    }

  }

  // 'use's are private by default, at least to external modules.  So
  // should 'sibling' be able to reference child's contents when it
  // 'use's 'parent' without its own use or import of child's
  // contents?  At present, the answer is no.  See issue #15314 for
  // further discussion.
  use child;

  module sibling {
    use parent;
    proc main() {
      writeln(secretFunction(11));

    }
  }
}
