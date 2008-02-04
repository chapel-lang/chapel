

class C {
  var x;
  var y;
}

var c = new C(1, 3.0);
var d = new C(2.0, 3.0);

def py(a) {
  writeln(a.y);
}
def px(a) {
  writeln(a.x);
}
px(c);
px(d);
py(c);
py(d);
