proc f(x: [] int, y: [] int) where x.rank == 1 && y.rank == 1 {
  var ret = x+y;
  return ret;
}

var xx: [1..10] int = 1;
var yy: [1..10] int = 2;

writeln(f(xx, yy));
