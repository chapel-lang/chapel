config const numRows: integer = 16;
config const numCols = 16;

const MatDom: domain(2) = [1..numRows, 1..numCols];

var Mat: [MatDom] real;

const numDiags: integer = min(numRows, numCols);

for d in 1..numDiags {
  const Reduced: domain(MatDom) = [d..numRows, d..numCols];
  // OR:
  //  const RedRows: domain(1) = d..numRows;
  //  const RedCols: domain(1) = d..numCols;
  //  const Reduced: domain(MatDom) = [RedRows, RedCols];
  const TopRow: domain(MatDom) = [d, 1..numCols];
  const LeftCol: domain(Reduced) = [d..numRows, d];
  // OR:
  //  const LeftCol: domain(Reduced) = [.., d]; -- ".. = inherit from parent dom."
  // ".." helps with compile-time subdomain checks...

  var pivotVal: real;
  var pivotLoc: index(LeftCol);
  (pivotVal, pivotLoc) = maxloc reduce abs(Mat(LeftCol));

  const PivotRow: domain(MatDom) = [pivotLoc(1), ..];

  swapRows(Mat, TopRow, PivotRow);
}

// need to swap *entire* rows, not just those in the Reduced submatrix
// - if L is stored in-line with A; if we stored it separately, we could
//   just do the swap on the reduced part of the row
function swapRows(Arr, Row1, Row2) {
  var TempRow: [Row1] real;

  TempRow = Arr(Row2);
  Arr(Row2) = Arr(Row1);
  Arr(Row1) = TempRow;
}

function swapRows(Arr, Row1, Row2) {
  var r1 = Row1.lo(1); // == Row1.hi(1);

  forall (r2, c) in Row2 {
    const tempVal = Arr(r2, c);
    Arr(r2, c) = Arr(r1, c);
    Arr(r1, c) = tempVal;
  }
}

function swapRows(Arr, Row1, Row2) {
  Arr(Row1) :=: Arr(Row2);
}


function swapRows(Arr: [], Row1: domain, Row2: domain): void {


// ideally...
var L: domain(MatDom) = [i in 1..n, 1..i-1];
var U: domain(MatDom) = [i in 1..n, i..n];


var L: domain(2) implemented (LowerTriangular) = [1..n, 1..n];
var U: domain(2) implemented (UpperTriangular) = [1..n, 1..n];
