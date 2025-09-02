use CTypes;
record R1 {
  var x: c_ptr(R2);
}
record R2 {
  var y: c_ptrConst(R1);
}
var r = new R1();
