record R {
  var n: int;
}

var r = new R(1);

class C {
  var a = r.n;
}

var c = new C();
r.n = 2;
var d = new C();

writeln(c);
writeln(d);

delete c;
delete d;
