module M {
  var x: int;

  module K {
    import super;

    proc main() {
      writeln(M.x);
    }
  }
}
