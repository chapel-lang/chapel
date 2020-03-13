proc shift(A, offset) where isRectangularArr(A) && A.rank==1 {
  const numElems = A.size;
  const ao = abs(offset);
  var sA = A;
  /* ran into internal error with the following */
  if true /*ao%numElems == 0*/ then return sA;
  var lo = A.domain.low;
  var hi = A.domain.high;
  for i in A.domain {
    var ii = i+offset;
    if ii > hi then ii-=numElems;
    else if ii < lo then ii+=numElems;
    sA[i] = A[ii];
  }
  return sA;
}

config const n=7;
var A: [3..n] int;
shift(A, 1);
