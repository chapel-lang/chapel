// The error being reported should point to the assignment,
// rather the record declaration.

record RECTYPE {
  var VARFIELD: int;
  const CONSTFIELD: int;
}

const D = {1..2};
var A: [D] RECTYPE;
A; // prevent split-init for this test as it focuses on error in = operator

// Error: cannot assign to 'A' because that would modify
// a 'const' field of A[1] etc.
A = [ new RECTYPE(1,1), new RECTYPE(2,2) ]; // <--- the error is here
