config var n : integer = 4;

record coord {
  var x : integer;
  var y : integer;
}

var d : domain(1) = [1..n];
var a : [d] coord;

[i in d] writeln(a(i).x, ", ", a(i).y);
