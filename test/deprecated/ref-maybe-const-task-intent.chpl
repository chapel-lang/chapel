var y: [1..10] int;
coforall i in 1..10 {
  y[i] = i;
}

var z: [1..10] int;
cobegin {
  z[1] = 1;
  z[2] = 2;
}

var w: [1..10] int;
sync {
  begin {
    w[1] = 1;
  }
}

proc bar() {
  var x: [1..10] int;
  proc foo() {
    var y: [1..10] int;
    cobegin {
      x[1] = 1;
      y[1] = 2;
    }
  }
  foo();
}
bar();

record R {
  var A: [1..10] int;
}
proc R.foo() {
  begin {
    this.A[1] = 1;
  }
}
var r = new R();
r.foo();
