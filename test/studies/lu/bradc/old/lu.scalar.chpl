// This is 

-- the problem size; these can be set on the executable's command-line
config const numRows: integer = 16;
config const numCols: integer = 16;


-- a domain to describe the matrix index space
const MatDom: domain(2) = [1..numRows, 1..numCols];


-- the matrix itself (a Chapel array)
var Mat: [MatDom] real;


-- the number of iterations we're going to run
const numDiags: integer = min(numRows, numCols);

for d in 1..numDiags {
  -- subdomains to describe different matrix substructures
  const Reduced: domain(MatDom) = [d..numRows, d..numCols];
  const TopRow: domain(Reduced) = [d, d..numCols];
  const LeftCol: domain(Reduced) = [d..numRows, d];

  -- compute the pivot value and location
  var pivotVal: real;
  var pivotLoc: index(LeftCol);
  (pivotVal, pivotLoc) = maxloc reduce Mat(LeftCol);

  -- define a subdomain describing the pivot row...
  const PivotRow: domain(Reduced) = [pivotLoc(1), d..numCols];

  -- ...and then perform the pivot
  swapRows(Mat, TopRow, PivotRow);

  -- now it looks like we need a new domain for LeftCol minus the
  -- first element:
  const ScaleCol: domain(LeftCol) = [d+1..numRows, d];
  -- or: const ScaleCol: domain(LeftCol) = nudgeLo(LeftCol, (-1, 0));

  -- compute multipliers/scale column
  Mat(ScaleCol) /= -pivotVal;

  -- eliminate pivot column by subtracting rows
  forall (i,j) in Reduced {
    Mat(i,j) += Mat(i,d) * Mat(d,j);
  }
}



-- here's one way to perform the pivot using whole-array assignment

function swapRows(Arr, Row1, Row2) {
  var TempRow: [Row1] real;

  TempRow = Arr(Row2);
  Arr(Row2) = Arr(Row1);
  Arr(Row1) = TempRow;
}


-- here's another way that swaps element one at a time (a
-- space/communication granularity tradeoff?)

function swapRows2(Arr, Row1, Row2) {
  var i1 = Row1.lo(1);

  forall (i2,j) in Row2 {
    const tempVal = Arr(i2, j);
    Arr(i2, j) = Arr(i1, j);
    Arr(i1, j) = tempVal;
  }
}


-- here's a third implementation that uses a(n undefined) swap
-- operator that Steve and I sometimes fantasize about

function swapRows3(Arr, Row1, Row2) {
  Arr(Row1) :=: Arr(Row2);
}
