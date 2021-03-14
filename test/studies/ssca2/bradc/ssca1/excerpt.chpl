
-- a domain defined by the lengths of the two sequences, extended to
-- support an extra initial row and column
const Table: domain(2) = {0..ILen, 0..JLen};

-- an array of sync integers (provides full/empty
-- semantics... initially empty)
sync var SWtab: [Table] integer;

-- initialize boundaries to full using a custom iterator and writes
forall (r,c) in NorthwestBoundary(Table) {
  SWtab(r,c) = 0;
}

-- iterate over table using custom iterator
forall (r,c) in AntiDiagonals(Table) {
  -- .read reads sync variable without emptying
  var neighborN  = SWtab(r-1, c  ).read,
      neighborW  = SWtab(r  , c-1).read,
      neighborNW = SWtab(r-1, c-1).read;

  SWtab(r,c) = SWCalc(r, c, neighborN, neighborW, neighborNW);
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
