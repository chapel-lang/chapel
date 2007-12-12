module M1 {
  record R { var x: int; }
  def R.foo() { writeln("R.foo"); }
}

module M2 {
  def bar(x) { x.foo(); }
}

module M3 {
  use M1, M2;
  def main() {
    var r: R;
    bar(r);
  }
}
