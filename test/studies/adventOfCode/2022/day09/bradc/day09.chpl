// features:
// - grow arrays by re-assigning domains
// - tuple math-y stuff (could I be even more clever, though?)
// - associative array (though plenty of other ways to do it)

use IO;

config const debug = false;

                   // what the site used


// This creates an arbitrarily-sized starting board, just based
// (loosely) on the picture in the instructions (I think it was 5 x 6?)
// We will grow it as we go...
//
config var n = 6;  // starting size of the array, just to almost match
var BoardSpace = {1..n, 1..n};
var Visited: [BoardSpace] bool;
//
// I'll be interpreting its coordinate scheme as follows:
//
// (n,1)      (n,n)
// :
// :
// (1,1)......(1,n)


var headloc, tailloc = (1,1);

var decoder = ["R" => (0,1), "L" => (0,-1), "U" => (1, 0), "D" => (-1, 0)];

proc main() {
  var line: string;
  while readLine(line, stripNewline=true) {
    debugBoard();
    var (dir, _, num) = line.partition(" ");
    if debug then writeln("Next command: ", (dir,num));

    for i in 1..num:int {
      headloc = headloc + decoder[dir];
      if !BoardSpace.contains(headloc) {
        growBoard(headloc);
      }
      computeTail();
      Visited[tailloc] = true;
    }
  }
  debugBoard();
  debugVisited();

  writeln(+ reduce Visited);
}


// re-assigning a domain in Chapel will re-allocate arrays declared over
// it (not cheap, but productive!  Gives us an arbitrarily growing canvas!
// (up to memory limits))
//
proc growBoard((r,c)) {
  const (rows,cols) = BoardSpace.dims();
  const (newrows, newcols) = (growRange(r,rows),growRange(c, cols));
  BoardSpace = {newrows, newcols};
}


// Grow the range if we fall outside of it; could use recursive doubling
// or something to amortize the overheads of reallocation...
//
proc growRange(i, rng) {
  if rng.contains(i) {
    return rng;
  } else {
    if i < rng.low {
      return rng.low-1..rng.high;  // there's probably a grow or extend methd
    } else {                       // I could use here?  can never quite recall
      return rng.low..rng.high+1;
    }
  }
}


proc computeTail() {
  var delta = headloc - tailloc;
  var tailstep = computeStep(delta);
  tailloc += tailstep;
}


proc computeStep(d) {
  const manDist = abs(d(0)) + abs(d(1));
  if manDist <= 2 {
    return (computeDiff(d(0)), computeDiff(d(1)));
  } else {
    return (computeCatchUp(d(0)), computeCatchUp(d(1)));
  }
}


proc computeDiff(d) {
  if (abs(d) <=1 ) {
    return 0;
  } else if d < 0 {
    return -1;
  } else if d > 0 {
    return 1;
  } else {
    halt("Impossible, I say!");
  }
}


proc computeCatchUp(d) {
  if d < 0 {
    return -1;
  } else if d > 0 {
    return 1;
  } else {
    halt("Shouldn't get here!");
  }
}


proc debugBoard() {
  if !debug then return;
  for r in BoardSpace.dim(0) by -1 {
    for c in BoardSpace.dim(1) {
      if headloc == (r,c) {
        write("H");
      } else if tailloc == (r,c) {
        write("T");
      } else if (r,c) == (1,1) {
        write("s");
      } else {
        write(".");
      }
    }
    writeln();
  }
  writeln();
}


proc debugVisited() {
  if !debug then return;
  for r in BoardSpace.dim(0) by -1 {
    for c in BoardSpace.dim(1) {
      if Visited[r,c] {
        write("#");
      } else if (r,c) == (1,1) {
        write("s");
      } else {
        write(".");
      }
    }
    writeln();
  }
  writeln();
}
