use IO;

const AlphaGrid = readGrid();

const numRows = AlphaGrid.size,
      numCols = AlphaGrid.first.size;

param a = b"a".toByte();

const Grid = {0..<numRows, 0..<numCols-1};

var Elevation: [Grid] int = [(i,j) in Grid] AlphaGrid[i][j] - a,
    Steps: [Grid] atomic int = max(int);

const (_, startLoc) = maxloc reduce zip((Elevation == (b"S".toByte() - a)), Grid),
      (_, endLoc) = maxloc reduce zip((Elevation == (b"E".toByte() - a)), Grid);

writeln(startLoc);
writeln(endLoc);

Elevation[startLoc] = 0;
Elevation[endLoc] = 25;

sync { Explore(startLoc); }

iter nextSteps() {
  yield (0,1);
  yield (1,0);
  yield (0,-1);
  yield (-1,0);
}

proc Explore(pos, elevation = Elevation[startLoc], steps = 0): int {
  do {
    var prevDist = Steps[pos].read();
    if steps >= prevDist {
      return;
    }
  } while (!Steps[pos].compareExchange(prevDist, steps));
  if pos == endLoc {
    return;
  }

  for off in nextSteps() {
    const nextPos = pos + off;
    if Grid.contains(nextPos) {
      const nextElev = Elevation[nextPos];
      if nextElev <= elevation || nextElev == elevation+1 {
        begin Explore(nextPos, nextElev, steps+1);
      }
    }
  }
}

//writeln(Steps);

writeln(Steps[endLoc]);

iter readGrid() {
  var line: bytes;
  while readLine(line) {
    yield line;
  }
}