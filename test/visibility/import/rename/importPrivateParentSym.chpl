module M {
  private var x = 42;

  module N {
    import M.{x as y};

    proc main() {
      writeln(y);
    }
  }
}
