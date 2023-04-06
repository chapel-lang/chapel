import IO.FormattedIO.format;
record Point {
  var x:int;
  var y:int;
}

var p = new Point(1,2);
var s = "%t".format(p);

var q:Point = s;
