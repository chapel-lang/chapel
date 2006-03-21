

class C {
  var x;
  var y;
}

var c = C();
var d = C();

c.x = 1;
d.x = 2.0;
c.y = 3.0;
d.y = 3.0;

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
