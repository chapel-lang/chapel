module M {
  private var x: int;
  private proc f() {
    writeln("in f");
  }
  module Sub {
    use Other;
    proc inner() {
      writeln("x is ", x);
      f();
    }
  }
  proc main() {
    use Sub;
    inner();
  }
}

module Other {
  public use M;
}
