proc swap(inout x, inout y) {
  var tmp = x;
  x = y;
  y = tmp;
}

var i : int = 2;
var j : int = 3;

writeln(i, ", ", j);

swap(i, j);

writeln(i, ", ", j);

var k : string = "juice";
var l : string = "water";

writeln(k, ", ", l);

swap(k, l);

writeln(k, ", ", l);

