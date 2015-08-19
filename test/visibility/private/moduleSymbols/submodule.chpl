module parent {
  private module child {
    proc secretFunction(a: int) {
      return a*3;
    }
  }

  proc main() {
    use child;
    writeln(secretFunction(11)); // Should return 33;
  }
}
