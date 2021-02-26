module M {
  var x = 42;
  var y: bool;

  module N {
    import super.{x, y};

    proc main() {
      writeln(x);
      writeln(y);
    }
  }
}
