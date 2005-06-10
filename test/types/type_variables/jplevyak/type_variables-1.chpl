
function f(type t, x) {
  var y : t = x;
  writeln(y);
}

var t = integer;
var tt = float;

f(t, 1);
f(tt, 1);

