class C {
  type t;
  var x : t;
}

var c = C(t = float);
var d : c.t;

d = 1;
writeln(d);
