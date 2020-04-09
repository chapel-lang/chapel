record R {
  param p : int;
  var x;
}

var r = new R(2, "hello world");

writeln("(p = ", r.p, ", x = ", r.x, ")");
