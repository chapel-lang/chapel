// the problem size; these can be set on the executable's command-line
config const numRows: integer = 16;
config const numCols: integer = 16;


// a domain to describe the matrix index space
const MatDom: domain(2) = [1..numRows, 1..numCols] dmapped Cyclic(2);


// the matrix itself (a Chapel array)
var Mat: [MatDom] real;


// the number of iterations we're going to run
const numDiags: integer = min(numRows, numCols);

for d in 1..numDiags {
  // subdomains to describe different matrix substructures
  const Reduced: subdomain(MatDom) = [d..numRows, d..numCols];
  const LeftCol: subdomain(Reduced) = [d..numRows, d];

  // compute the pivot value and location
  var pivotVal: real;
  var pivotLoc: index(LeftCol);
  (pivotVal, pivotLoc) = maxloc reduce abs(Mat(LeftCol));

  // ...and then perform the pivot
  swapRows(Mat, row1=d, row2=pivotLoc(1));

  // now it looks like we need a new domain for LeftCol minus the
  // first element:
  const ScaleCol: subdomain(LeftCol) = [d+1..numRows, d];
  // or: const ScaleCol: domain(LeftCol) = [d+1.. , ..]; to inherit stuff?
  // or: const ScaleCol: domain(LeftCol) = nudgeLoBound(LeftCol, (-1, 0));

  // compute multipliers/scale column
  Mat(ScaleCol) /= -pivotVal;

  // eliminate pivot column by subtracting rows
  forall (r,c) in Reduced {
    Mat(r,c) += Mat(r,d) * Mat(d,c);
  }
}



// here's one way to perform the pivot using whole-array assignment
// arguments are:
// - the array of values
// - the subdomain of the array where rows should be swapped (by
//   default, it's the array's declared domain)
// - the row numbers that should be swapped
function swapRows(Arr, Dom = Arr.domain, row1, row2) {
  const Row1: subdomain(Dom) = [row1, ..];
  const Row2: subdomain(Dom) = [row2, ..];
  const TempRow: [Row1] real = Arr(Row2);

  Arr(Row2) = Arr(Row1);
  Arr(Row1) = TempRow;
}


// here's another way that swaps element one at a time (a
// tradeoff between temp space and communication granularity?)

function swapRows2(Arr, Dom = Arr.domain, row1, row2) {
  forall col in Dom(2) {
    const tempVal = Arr(row2, col);
    Arr(row2, col) = Arr(row1, col);
    Arr(row1, col) = tempVal;
  }
}


// here's a third implementation that uses a(n undefined) swap
// operator that Steve and I sometimes fantasize about

function swapRows3(Arr, Dom = Arr.domain, row1, row2) {
  const Row1: subdomain(Dom) = [row1, ..];
  const Row2: subdomain(Dom) = [row2, ..];
  Arr(Row1) <=> Arr(Row2);
}
