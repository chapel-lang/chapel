fun ScalarLU(Mat: [?MatDom] ?t) {
  var (numrows, numcols) = MatDom.extent();

  // the number of iterations we're going to run
  const numDiags = min(numRows, numCols);

  // permutation array for tracking pivots
  var pivotArr: [1..numRows] int = [i in 1..numRows] i;

  // subdomains to describe different matrix substructures
  var Reduced: subdomain(MatDom) = MatDom;
  var LeftCol: subdomain(MatDom) = Reduced.interior(0, 1);
  // note: would like to make LeftCol a subdomain of Reduced, but
  // there's a subtle timing/commit issue

  for d in 1..numDiags {
    // compute the pivot value and location
    var pivotVal: t;
    var pivotLoc: index(LeftCol);
    (pivotVal, pivotLoc) = maxloc reduce abs(Mat(LeftCol));
  
    // ...and then perform the pivot
    swapRows(Mat, row1=d, row2=pivotLoc(1));
    pivotArr(d) <=> pivotArr(pivotLoc(1));

    // now it looks like we need a new domain for LeftCol minus the
    // first element:
    const ScaleCol: subdomain(LeftCol) = LeftCol.nudgeLo(1, 0);  
    // or: const ScaleCol: subdomain(LeftCol) = [d+1..numRows, d];
    // or: const ScaleCol: subdomain(LeftCol) = [d+1.. , ..]; to inherit stuff?
    // or better:                             = LeftCol[d+1.., ..];

    // compute multipliers/scale column
    Mat(ScaleCol) /= -pivotVal;

    // eliminate pivot column by subtracting rows
    forall (r,c) in Reduced {
      Mat(r,c) += Mat(r,d) * Mat(d,c);
    }

    Reduced = Reduced.nudgeLo(-1,-1);
    LeftCol = Reduced.interior(0, 1);
  }

  return (Mat, pivotArr);
}



// here's one way to perform the pivot using whole-array assignment
// arguments are:
// - the array of values
// - the subdomain of the array where rows should be swapped (by
//   default, it's the array's declared domain)
// - the row numbers that should be swapped
fun swapRows(Arr, Dom = Arr.domain, row1, row2) {
  const Row1: subdomain(Dom) = [row1, ..];
  const Row2: subdomain(Dom) = [row2, ..];
  const TempRow: [Row1] real = Arr(Row2);

  Arr(Row2) = Arr(Row1);
  Arr(Row1) = TempRow;
}


// here's another way that swaps element one at a time (a
// tradeoff between temp space and communication granularity?)

fun swapRows2(Arr, Dom = Arr.domain, row1, row2) {
  forall col in Dom(2) {
    const tempVal = Arr(row2, col);
    Arr(row2, col) = Arr(row1, col);
    Arr(row1, col) = tempVal;
  }
}


// here's a third implementation that uses a(n undefined) swap
// operator that Steve and I sometimes fantasize about

fun swapRows3(Arr, Dom = Arr.domain, row1, row2) {
  const Row1: subdomain(Dom) = [row1, ..];
  const Row2: subdomain(Dom) = [row2, ..];
  Arr(Row1) <=> Arr(Row2);
}
