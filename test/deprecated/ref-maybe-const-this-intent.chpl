record R {
  var x: int = 11;
  proc foo() {
    x = 10;
  }
}
var r = new R();
r.foo();
