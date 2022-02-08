use IO;

var xmax, ymax: int;

iter readCoords() {
  do {
    var x1, y1, x2, y2: int;
    const success = readf("%i,%i -> %i,%i", x1, y1, x2, y2);
    if success {
      if x1 > xmax then xmax = x1;
      if x2 > xmax then xmax = x2;
      if y1 > ymax then ymax = y1;
      if y2 > ymax then ymax = y2;
      yield ((x1,y1),(x2,y2));
    }
  } while success;
}

var coords = readCoords();
config const debug = (max(xmax,ymax) < 20);
if debug then writeln(coords);

param X = 0, Y = 1;

/*
var xmax = max(max reduce coords(0)(X), max reduce coords(1)(X));
var ymax = max(max reduce coords(0)(Y), max reduce coords(1)(Y));

writeln({0..xmax, 0..ymax});
*/

var count: [0..xmax, 0..ymax] atomic int;

forall ((x1,y1),(x2,y2)) in coords {
  const minx = min(x1, x2);
  const maxx = max(x1, x2);
  const miny = min(y1, y2);
  const maxy = max(y1, y2);
//  writeln("Doing: ", (x1,x2), " ", (y1, y2));
  if x1 == x2 {
    if y1 <= y2 {
      count[x1..x2, y1..y2].add(1);
    }  else {
      count[x1..x2, y2..y1].add(1);
    }
  } else if y1 == y2 {
    if x1 <= x2 {
      count[x1..x2, y1..y2].add(1);
    }  else {
      count[x2..x1, y1..y2].add(1);
    }
  } else {
    const xstride = if x1 < x2 then 1 else -1;
    const ystride = if y1 < y2 then 1 else -1;
//    writeln("Trying ", (x1..x2 by xstride), " ", (y1..y2 by ystride));
//    writeln((x1..x2 by xstride).size);
//    writeln((y1..y2 by ystride).size);
    for (x,y) in zip(minx..maxx by xstride, miny..maxy by ystride) do
      count[x,y].add(1);
  }
}

if debug then writeln(count);

writeln(+ reduce (count.read() >= 2));

