module M {
  private var x = 42;

  module N {
    import M.x;

    proc main() {
      writeln(x);
    }
  }
}