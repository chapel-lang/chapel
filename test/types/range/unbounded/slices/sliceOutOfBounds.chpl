var r1 = 1..7;
var r2 = 8..;
var A: [r1] real;
writeln(r1.chpl_boundsCheck(r2)); // should print false
writeln(A[r2]);                   // should halt
