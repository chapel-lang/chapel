class C {
  type t;
  var x : t;
}

var c = C(t = real);
var d : c.t;

d = 1;
writeln(d);
