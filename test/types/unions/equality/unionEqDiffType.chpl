config type t = u2;

union u0 {
  var x: int;
  var y: int;
  var z: real;
}

union u2 {
  var a: int;
  var b: real;
  var c: string;
}

union u3 {
  var x: int;
  var y: int;
  var z: real;
}

var ua: u0;
var ub: t;

writeln(ua == ub);
