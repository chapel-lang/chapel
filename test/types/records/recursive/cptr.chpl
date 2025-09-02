use CTypes;
record R {
  var x: c_ptr(R);
}
var r = new R();
