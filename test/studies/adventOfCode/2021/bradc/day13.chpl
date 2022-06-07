use IO;

// TODO: I spent a lot of time confused about X vs. Y on this because
// I usually think in terms of rows then columns.  I'm sure I got the
// right answer, but not convinced I was consistent in my ordering
// here, so that is a potential place for clean-up.

// Also, it seems as though I solved both problems in the same program
// this time around for whatever reason.

// --> X
// |
// v
// Y

param X = 1, Y = 0;

config const debug = false;

var Dots = readDots();
if debug then
  writeln(Dots);
var Folds = readFolds();
if debug then
  writeln(Folds);

if debug {
  writeln(Dots[X]);
  writeln(Dots[Y]);
}

var xmax = max reduce [d in Dots] d[X],
    ymax = max reduce [d in Dots] d[Y];

writeln("initial sheet size is ", (ymax, xmax));
const D = {0..ymax, 0..xmax};
var Sheet: [D] bool;
forall (y,x) in Dots do
  Sheet[y,x] = true;
//printSheet(Sheet);

foldSheet(Sheet);

iter readDots() {
  var line: string;
  do {
    readLine(line);
    var A = line.strip().split(",");
    if A.size == 2 then
      yield (A[0]:int, A[1]:int);
  } while A.size == 2;
}

iter readFolds() {
  var line: string;
  try {
    var fold: string;
    while readf("fold along %s\n", fold) {
      yield fold;
    }
  } catch { }
}

proc printSheet(Sheet: [?D]) {
  if debug || (D.dim(Y).size < 80) {
    for x in D.dim(X) {
      for y in D.dim(Y) {
        if Sheet[y, x] {
          write("#");
        } else {
          write(".");
        }
      }
      writeln();
    }
  }
  writeln();
}

proc foldSheet(Sheet: [?Dold], step=0) {
  if step >= Folds.size {
    printSheet(Sheet);
    return;
  }
  writeln(Folds[step][0]);
  var horizontal = Folds[step][0] == "x";
  var vertical = !horizontal;
  if horizontal then writeln("horizontal"); else writeln("vertical");
  var oldx = Dold.dim(X).size, oldy = Dold.dim(Y).size;;
  writeln((oldy,oldx));
  var foldplace = Folds[step][2..]:int;
  writeln("Folding at ", foldplace);
  var newy = oldy, newx = oldx;
  if horizontal {
    newy = foldplace;
//    assert(foldplace == oldy / 2);
  } else {
    newx = foldplace;
    //    assert(foldplace == oldx / 2);
  }

  // 7 , 3
  
//  var newy = (oldy - horizontal) / (1 + horizontal);
//  var newx = (oldx - vertical) / (1 + vertical);
  writeln((newy,newx));
  var Dnew = {0..<newy, 0..<newx};
  var FoldedSheet: [Dnew] bool;
  proc safeAccess(y, x) {
    if Sheet.domain.contains((y,x)) then
      return Sheet(y,x);
    else
      return false;
  }
  if horizontal then
    forall (y,x) in Dnew do
      FoldedSheet[y,x] = Sheet[y,x] || safeAccess(newy + (newy-y),x);
  else
    forall (y,x) in Dnew do
      FoldedSheet[y,x] = Sheet[y,x] || safeAccess(y,newx + (newx-x));
  printSheet(FoldedSheet);
  writeln(+ reduce FoldedSheet);
  foldSheet(FoldedSheet, step+1);
}
