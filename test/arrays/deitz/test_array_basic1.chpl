config var n : int = 4;

record coord {
  var x : int;
  var y : int;
}

var a : [1..n] coord;

[i in 1..n] writeln(a(i).x, ", ", a(i).y);
