module M {

  module N {
    import super.O.x;

    proc main() {
      writeln(x);
    }
  }

  module O {
    var x = 42;
  }
}