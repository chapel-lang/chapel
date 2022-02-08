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

forall ((x1,y1),(x2,y2)) in coords do
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
  }

if debug then
  writeln(count);

writeln(+ reduce (count.read() >= 2));

