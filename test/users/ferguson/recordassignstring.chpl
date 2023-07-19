import IO.FormattedIO.format;
record Point {
  var x:int;
  var y:int;
}

var p = new Point(1,2);
var s = "%?".format(p);

var q:Point = s;
