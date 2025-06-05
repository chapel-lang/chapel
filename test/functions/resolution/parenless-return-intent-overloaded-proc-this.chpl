record R {}
var r = new R();
var myConstant = 1.0;

proc R.foo {
  return myConstant;
}
proc R.foo ref {
  return myConstant;
}

r.foo();
