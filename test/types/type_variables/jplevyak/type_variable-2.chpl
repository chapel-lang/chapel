class C {
  type t;
  var x : t;
}

var c = new C(t = real);
var d : c.t;

d = 1;
writeln(d);

delete c;
