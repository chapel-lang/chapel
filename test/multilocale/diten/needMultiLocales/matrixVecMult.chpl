use BlockDist;

config const rows = 100;
config const cols = 75;

const RowDom: domain(1) = {1..cols};
const ColumnDom: domain(1) = {1..rows};

const BlockedDist = new dmap(new Block(rank=1,boundingBox=ColumnDom));
const DistColumnDom: domain(1) dmapped BlockedDist = ColumnDom;

var vec: [RowDom] real;
var resultVec: [ColumnDom] real;

var matrix: [DistColumnDom] [RowDom] real;

proc init(X) { /* ... */ }

init(matrix);
init(vec);

forall i in DistColumnDom {
  var result = 0.0;
  var localVec = vec; // Copy the input vector to the current locale
  for j in RowDom {
    result += matrix(i)(j) * localVec(j);
  }
  resultVec(i) = result;
}

writeln(resultVec);
