class E {
  var n: int;
}

var e = new E(1);

class C {
  var a = e.n;
}

var c = new C();
e.n = 2;
var d = new C();

writeln(c);
writeln(d);

delete e;
delete c;
delete d;
