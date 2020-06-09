module A {
  record R {
    var x: int;
    proc foo() { return x; }
  }
}

var r1: A.R; // error: unresolved call 'R.init()'
var r2 = new A.R(); // Ok
writeln(r1.foo());
writeln(r2.foo());
