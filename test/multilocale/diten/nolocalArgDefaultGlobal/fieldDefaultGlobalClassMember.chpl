class E {
  var n: int;
}

var e = new owned E(1);

class C {
  var a = e.n;
}

var c = new owned C();
e.n = 2;
var d = new owned C();

writeln(c);
writeln(d);
