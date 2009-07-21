record R {
  var n: int;
  def R(n: int) {
    this.n = n;
    writeln("Construct R: ", this.n);
  }
  def ~R() {
    writeln("Destroy R: ", this.n);
  }
}

def f() {
  var r1 = new R(1);
  var r2 = new R(2);
  r2 = r1;
}

def main {
  f();
}
