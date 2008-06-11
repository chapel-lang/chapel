class C {
  type ct;
  type bt = real;
  var x: ct;
  var y: bt;

  def C(type ct, type bt, y: ct) {
    x = y;
  }

  def C(type ct, y: ct) {
    x = y;
  }
}

var c = new C(int, y=2);
writeln(c);
var d = new C(int, int, y=2);
writeln(d);
