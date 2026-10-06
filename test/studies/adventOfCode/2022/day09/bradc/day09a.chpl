







use IO;

config const debug = false;

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


var rope: [0..9] (int,int) = (1,1);

var decoder = ["R" => (0,1), "L" => (0,-1), "U" => (1, 0), "D" => (-1, 0)];

proc main() {
  var line: string;
  while readLine(line, stripNewline=true) {
    debugBoard();
    var (dir, _, num) = line.partition(" ");
    if debug then writeln("Next command: ", (dir,num));

    for i in 1..num:int {
      rope[0] = rope[0] + decoder[dir];
      if !BoardSpace.contains(rope[0]) {
        growBoard(rope[0]);
      }
      computeTail();
      Visited[rope[9]] = true;
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
  for i in 1..9 {
    var delta = rope[i-1] - rope[i];
    var tailstep = computeStep(delta);
    rope[i] += tailstep;
  }
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
      if rope[0] == (r,c) {
        write("H");
      } else {
        var found = false;
        for i in 1..9 {
	  if rope[i] == (r,c) {
	    write(i);
	    found = true;
	    break;
	  }
	}
	if !found {
          if (r,c) == (1,1) {
        write("s");
      } else {
        write(".");
      }
    }
    }
    }
    writeln();
  }
  writeln();
}

proc debugVisited() {
  if !debug then return;
  for r in 1..n by -1 {
    for c in 1..n {
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
