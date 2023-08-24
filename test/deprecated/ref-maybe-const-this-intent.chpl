record R {
  var x: int = 11;
  var y: int = 12;
  proc foo() {
    x = 10;
  }
  proc bar() {
    writeln(y);
  }
}
var r = new R();
r.foo();
r.bar();
