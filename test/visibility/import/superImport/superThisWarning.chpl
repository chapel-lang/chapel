module foo {
  var x: int;

  module M {
    public import super.this;

    proc main() {
      writeln(foo.x);
    }
  }
}
