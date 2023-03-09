use IO;

config const debug = false,
             version = 0,
             printTaskCount = false;

const AlphaGrid = readGrid();

const numRows = AlphaGrid.size,
      numCols = AlphaGrid.first.size;

param a = b"a".toByte();

const Grid = {0..<numRows, 0..<numCols-1};

var Elevation: [Grid] int = [(i,j) in Grid] AlphaGrid[i][j] - a,
    Steps: [Grid] atomic int = max(int);

const (_, startLoc) = maxloc reduce zip((Elevation == (b"S".toByte() - a)), Grid),
      (_, endLoc) = maxloc reduce zip((Elevation == (b"E".toByte() - a)), Grid);

Elevation[startLoc] = 0;
Elevation[endLoc] = 25;

var numTasks: atomic int;

sync { Explore(startLoc); }

iter nextSteps() {
  yield (0,1);
  yield (1,0);
  yield (0,-1);
  yield (-1,0);
}

iter nextPositions(pos, elevation, in steps) {
  // we're seeing what happens if we take a step, so let's just take
  // that now
  steps += 1;

  // check if there's no need to go on b/c someone has already gotten
  // there faster than us
  if steps >= Steps[endLoc].read() then
    return;

  label outer for off in nextSteps() {
    const nextPos = pos + off;

    // Only proceed if the position is in the grid...
    if Grid.contains(nextPos) {

      // Only proceed if the elevation is legal
      const nextElev = Elevation[nextPos];
      if nextElev > elevation+1 then
        continue;

      // Bail out if someone has gotten here faster than us
      var prevDist = Steps[nextPos].read();
      do {
        if steps >= prevDist then
          continue outer;
      } while !Steps[nextPos].compareExchange(prevDist, steps);

      yield (nextPos, nextElev);
    }
  }
}

proc Explore(pos, elevation = Elevation[startLoc], steps = 0): int {
  numTasks.add(1);

  if pos == endLoc then
    return;

  if version == 0 {

    // Asynchronous version using 'begin'
    for (nextPos, nextElev) in nextPositions(pos, elevation, steps) do
      begin with (in nextPos) { Explore(nextPos, nextElev, steps+1); }

  } else if version == 1 {

    // Asynchronous version using 'begin' that doesn't, seemingly due
    // to either sharing 'nextPos' or corrupting it somehow...?
    for (nextPos, nextElev) in nextPositions(pos, elevation, steps) do

      begin { Explore(nextPos, nextElev, steps+1); }
  } else if version == 2 {

    // A bit more synchronous than the 'begin' version but makes far
    // fewer recursive calls on average...?
    coforall (nextPos, nextElev) in nextPositions(pos, elevation, steps) do
      Explore(nextPos, nextElev, steps+1);

  } else {
    // Serial version
    for (nextPos, nextElev) in nextPositions(pos, elevation, steps) do
      Explore(nextPos, nextElev, steps+1);
  }    
}

//writeln(Steps);

writeln(Steps[endLoc]);
if printTaskCount then
  writeln("numTasks = ", numTasks.read());  // 2686803 for the real input, serially

iter readGrid() {
  var line: bytes;
  while readLine(line) {
    yield line;
  }
}
