record Coord {
  var x: int;
  var y: int;
  var z: int;
}

var D: domain(Coord);

var c1: Coord = new Coord(0, 0, 1);
var c2: Coord = new Coord(0, 1, 0);
var c3: Coord = new Coord(1, 0, 0);

D += c1;
D += c2;
D += c3;

var Name: [D] string;

Name(c1) = "z basis coordinate";
Name(c2) = "y basis coordinate";
Name(c3) = "x basis coordinate";

for coord in D.sorted() {
  writeln("coord ", coord, " is called ", Name(coord));
}

proc <(a: Coord, b: Coord) {
  if (a.x == b.x) {
    if (a.y == b.y) {
      if (a.z == b.z) {
        return false;
      } else {
        return (a.z < b.z);
      }
    } else {
      return (a.y < b.y);
    }
  } else {
    return (a.x < b.x);
  }
}
