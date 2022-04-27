module bar {
  var x: int;

  module foo {
    module innermost {
      proc main() {
        import super.super;
        writeln(bar.x);
      }
    }
  }
}
