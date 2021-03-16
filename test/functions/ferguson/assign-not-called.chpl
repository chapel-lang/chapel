record R {
  var x:int;
}

operator R.=(ref lhs:R, rhs:R) {
  compilerError("boo");
}

var r:R = new R(1);

