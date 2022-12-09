use IO;

config var n = 6;

enum piece {o, T, H};

var BoardSpace = {1..n, 1..n};

//var Board: [BoardSpace] piece;
var Visited: [BoardSpace] bool;

var headloc = (1,1);
var tailloc = (1,1);

printBoard();

var decoder = ["R" => (0,1), "L" => (0,-1), "U" => (1, 0), "D" => (-1, 0)];

var line: string;
writeln("Before loop!");
while readLine(line, stripNewline=true) {
  writeln("in loop!");
  var (dir, _, num) = line.partition(" ");
  writeln((dir,num));

  for i in 1..num:int {
    headloc = headloc + decoder[dir];
    if !BoardSpace.contains(headloc) {
      writeln("headloc out of bounds: ", headloc);
      growBoard(headloc);
    }
    computeTail();
//    printBoard();
  }
}
printVisited();
writeln(+ reduce Visited);

proc growBoard((r,c)) {
  const (rows,cols) = BoardSpace.dims();
  var (newrows, newcols) = (rows, cols);
  if !rows.contains(r) {
    if r < rows.low {
      newrows = rows.low-1..rows.high;
    } else {
      newrows = rows.low..rows.high+1;
    }
  }
  if !cols.contains(c) {
    if c < cols.low {
      newcols = cols.low-1..cols.high;
    } else {
      newcols = cols.low..cols.high+1;
    }
  }
  writeln("Regrowing board to ", (newrows, newcols));
  BoardSpace = {newrows, newcols};
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

proc computeStep(d) {
  const manDist = abs(d(0)) + abs(d(1));
  if manDist <= 2 {
    return (computeDiff(d(0)), computeDiff(d(1)));
  } else {
    writeln("Here's the hard case");
    return (computeCatchUp(d(0)), computeCatchUp(d(1)));
//    return (computeDiff(d(0)), computeDiff(d(1)));
//    return (0,0);
  }
}

proc computeTail() {
  var delta = headloc - tailloc;
  writeln(delta);
  var tailstep = computeStep(delta);
  writeln(tailstep);
  tailloc += tailstep;
  Visited[tailloc] = true;
}

proc printBoard() {
  for r in 1..n by -1 {
    for c in 1..n {
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

proc printVisited() {
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

/*
var A = readIt();
writeln(A);

iter readIt() {
  var s: string;
  while (readLine(s)) {
    yield s;
  }
}*/