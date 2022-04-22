module bar {
  var x: int;

  module foo {
    module innermost {
      proc main() {
        import super.this.super;
        writeln(bar.x);
      }
    }
  }
}
