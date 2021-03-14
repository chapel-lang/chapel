record R {
  var x:int;
}

proc =(ref lhs:R, rhs:R) {
  compilerError("boo");
}

var r:R = new R(1);

