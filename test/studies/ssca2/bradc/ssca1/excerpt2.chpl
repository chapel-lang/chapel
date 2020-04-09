
-- an enumeration of the possible backtrace directions
enum direction = { north, west, northwest};

-- a domain over backtrace directions
const Directions: domain(direction);

-- an array of offsets corresponding to each backtrace direction
const offset: [Directions] 2*integer = (/ (-1,0), (0,-1), (-1,-1) /);

-- a 2D space defined by the lengths of the two sequences, extended to
-- support an additional initial (0th) row and column
const SeqSpace: domain(2) = {0..ILen, 0..JLen};

-- an array of sync integers ("sync" results in full/empty
-- semantics... initially empty)
sync var SWtab: [SeqSpace] integer;

-- the array of backTrace directions for the sequence space
var backTrace: [SeqSpace] direction;

-- initialize boundaries to full using a custom iterator and writes
forall (r,c) in NorthwestBoundary(SeqSpace) {
  SWtab(r,c) = 0;
}

-- iterate over table via anti-diagonals using custom iterator
forall loc in AntiDiagonals(SeqSpace) {
  -- create array of neighboring values
  -- .read reads sync variable without emptying
  var Neighbors: [Directions] integer = [d in Directions] SWtab(loc + offset(d)).read;

  SWtab(loc) = SWCalc(loc, Neighbors, backTrace);
}

-- use a reduction to compute the location/index of the maximum value
-- in the table
var loc: index(SeqSpace) = maxloc reduce SWtab;

-- walk back through the table
while (SWtab(loc) != 0) {
  loc += offset(backTrace(loc));
  saveTrace(loc);
}


-- this iterator returns all indices in the 0th row and column (and,
-- due to laziness, returns (0,0) twice)

proc NorthwestBoundary(D: domain(2)) {
  cobegin {
    forall r in D(*,0) {
      yield (r,0);
    }
    forall c in D(0,*) {
      yield (0,c);
    }
  }
}


-- this iterator iterates over a rectangular domain by antidiagonals

proc AntiDiagonals(D: domain(2) {
  const numDiags = D.extent(1) + D.extent(2) - 1;
  for d in 1..numDiags {
    forall i in 1..lenDiag(d) {
      var (r,c) = diagsToCoords(d, i);
      yield (r,c);
    }
  }
}
