
record R { 
  type t;
  var x : integer;
}

var r = R(t = integer, x = 1);

var n : r.t;

writeln(r.x);
writeln(n);
