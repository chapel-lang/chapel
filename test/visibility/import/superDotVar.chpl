module M {
  var x = 42;

  module N {
    import super.x;

    proc main() {
      writeln(x);
    }
  }
}