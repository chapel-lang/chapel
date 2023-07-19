import IO.FormattedIO.format;

record Point {
  var x: int;
  var y: int;
}

var p = new Point(1,3);
var s = "%?".format(p);

writeln(s);

var q = s:Point;

assert(p.x == q.x);
assert(p.y == q.y);
