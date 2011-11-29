
record R { 
  type t;
  var x : int;
}

var r = new R(t = int, x = 1);

var n : r.t;

writeln(r.x);
writeln(n);
