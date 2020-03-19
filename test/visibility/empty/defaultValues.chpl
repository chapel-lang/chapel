module OuterModule {
  module A {
    record R {
      type t;
    }
  }

  proc main() {
    use this.A only;
    var r: A.R(int);
    writeln(r.type: string);
  }
}
