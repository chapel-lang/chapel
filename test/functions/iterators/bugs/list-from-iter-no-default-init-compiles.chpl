// bug report from issue #21262
// see list-from-iter-no-default-init.chpl for code along these lines that runs

use IO, FileSystem, FormattedIO, List;

config const inf = "input.txt";

type Point = (int, int);

record Beacon {
    const location: Point, beacon: Point;

    proc init(location: Point, beacon: Point) {
        this.location = location;
        this.beacon = beacon;
    }

    proc beaconDistance(): int {
        return abs(this.location[0] - this.beacon[0]) + abs(this.location[1] - this.location[0]);
    }
}

iter loadInput() {
    var a, b, c, d: int;
    const f = open(inf, ioMode.r).reader();
    while f.readf("Sensor at x=%i, y=%i: closest beacon is at x=%i, y=%i\n", a, b, c, d) {
        yield new Beacon((a, b), (c, d));
    }
}

proc blockedCells(ycoord: int, beacons: [?]Beacon): int {
    return 0;
}

proc main() {
    const beacons: list(Beacon) = loadInput();
    writeln(beacons);
}
