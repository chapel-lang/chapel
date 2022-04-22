module bar {
  var x: int;

  module foo {
    module innermost {
      proc main() {
        import super.super.this.this;
        writeln(bar.x);
      }
    }
  }
}
