

class C {
  var x;
  var y;
}

var c = C(1, 3.0);
var d = C(2.0, 3.0);

fun py(a) {
  writeln(a.y);
}
fun px(a) {
  writeln(a.x);
}
px(c);
px(d);
py(c);
py(d);
