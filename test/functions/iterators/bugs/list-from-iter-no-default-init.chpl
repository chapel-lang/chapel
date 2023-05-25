// related to issue #21262

use List;

type Point = (int, int);

record Beacon {
  const location: Point, beacon: Point;

  proc init(location: Point, beacon: Point) {
    this.location = location;
    this.beacon = beacon;
  }
}

iter makeBeacons() {
  for i in 0..<10 {
    yield new Beacon((i, i+1), (i+2, i+3));
  }
}

proc main() {
  const beacons: list(Beacon) = makeBeacons();
  for elt in beacons {
    writeln(elt);
  }
}
