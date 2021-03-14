record R {
  var n: int;
  proc init(n: int) {
    this.n = n;
    writeln("Construct R: ", this.n);
  }
  proc deinit() {
    writeln("Destroy R: ", this.n);
  }
}

proc f() {
  var r1 = new R(1);
  var r2 = new R(2);
  r2 = r1;
}

proc main {
  f();
}
