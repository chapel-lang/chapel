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

/*
writeln(startLoc);
writeln(endLoc);
*/

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
  if steps >= Steps[endLoc].read() then {
    if debug then
    writeln("returning early due to taking ", steps, " while someone else ",
             "finished in ", Steps[endLoc].read());
            
    return;
  }

  label outer for off in nextSteps() {
    var msg: string;

    const nextPos = pos + off;
    if debug then
      msg += "Trying " + nextPos:string + " from " + pos:string + "\n";

    // If the elevation is legal, yield the step
    if Grid.contains(nextPos) {
    if debug then
      msg += "  It's in the grid\n";
      const nextElev = Elevation[nextPos];
      if nextElev > elevation+1 then {
        if debug then
          writeln(msg + "  elevation is too high...continuing");
        continue;
      }

      // If someone has gotten to this nextPos faster than us, return
      var prevDist = Steps[nextPos].read();
      do {
        if steps >= prevDist then {
          if debug then
            writeln(msg + "  someone got here faster...continuing");
          continue outer;
        }
      } while !Steps[nextPos].compareExchange(prevDist, steps);

      if debug then
        writeln(msg + "  Yielding ", (nextPos, nextElev));
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
    for (nextPos, nextElev) in nextPositions(pos, elevation, steps) do {
//      writeln("Exploring ", nextPos);
      begin with (in nextPos) { Explore(nextPos, nextElev, steps+1); }
    }
  } else if version == 1 {
    // Asynchronous version using 'begin' that doesn't, seemingly due
    // to either sharing 'nextPos' or corrupting it somehow...?
    for (nextPos, nextElev) in nextPositions(pos, elevation, steps) do {
//      writeln("Exploring ", nextPos);
      begin { Explore(nextPos, nextElev, steps+1); }
    }
  } else if version == 2 {
    // More synchronous version...
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
