config var n : integer = 4;

record coord {
  var x : integer;
  var y : integer;
}

var a : [1..n] coord;

[i in 1..n] writeln(a(i).x, ", ", a(i).y);
