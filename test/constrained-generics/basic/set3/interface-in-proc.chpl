interface I {}
proc foo(i) {
  record R: I {}

  var r: R;
  param p =  __primitive("implements interface", r, I);
  compilerAssert(p == 0);
}
foo(2);
