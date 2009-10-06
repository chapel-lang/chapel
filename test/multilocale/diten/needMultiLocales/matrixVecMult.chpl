use BlockDist;

config const rows = 100: int(64);
config const cols = 75: int(64);

const RowDom: domain(1, int(64)) = [1..cols];
const ColumnDom: domain(1, int(64)) = [1..rows];

const BlockedDist = distributionValue(new Block(rank=1,bbox=ColumnDom));
const DistColumnDom: domain(1, int(64)) distributed BlockedDist = ColumnDom;

var vec: [RowDom] real;
var resultVec: [ColumnDom] real;

var matrix: [DistColumnDom] [RowDom] real;

def init(X) { /* ... */ }

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
