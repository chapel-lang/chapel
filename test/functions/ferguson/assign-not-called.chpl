record R {
  var x:int;
}

operator =(ref lhs:R, rhs:R) {
  compilerError("boo");
}

var r:R = new R(1);

