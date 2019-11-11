use IO;

record Point {
  var x: int;
  var y: int;
}

var f = opentmp();
var w = f.writer();

var p = new Point(1,3);

w.writef("Hello %<t there\n", p);
w.close();

var r = f.reader();

p.x = 0;
p.y = 0;
r.readf("Hello %<t there\n", p);

assert( p.x == 1 );
assert( p.y == 3 );

