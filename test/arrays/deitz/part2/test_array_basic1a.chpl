config var n : int = 4;

record coord {
  var x : int;
  var y : int;
}

var d : domain(1) = {1..n};
var a : [d] coord;

[i in d] writeln(a(i).x, ", ", a(i).y);
