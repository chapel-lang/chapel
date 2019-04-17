
use BlockDist;

proc main() {
  var D = {1..10, 1..10};
  var S = D dmapped Block(D);
  var A, B : [S] int;

  var success = chpl__bulkTransferArray(A, B);

  assert(!success);
}
