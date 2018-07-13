
proc foo() type {
  return R(int);
}

record R {
  type T;
  var r : foo();
}

var r = new R(int);
