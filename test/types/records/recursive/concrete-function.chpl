
proc foo() type {
  return R;
}

record R {
  var x : foo();
}

var r = new R();
