config const n = 5;

var D: domain(2) = {1..n, 1..n};
var A: [D] int;

if storageOrder == ArrayStorageOrder.CMO {
  var val: int = 1;
  for i in 1..n {
    for j in 1..n {
      A[j, i] = val;
      val += 1;
    }
  }
} else {
  var val: int = 1;
  for i in 1..n {
    for j in 1..n {
      A[i,j] = val;
      val += 1;
    }
  }
}

// The numbers printed should be consecutive here because
// we walked the arrays in the right order above.
for i in 0..n*n-1 {
  writeln(A._value.data[i]);
}

