var r1 = 1..5 by 2 align 1;
var r2 =  ..  by 2 align 0;
var A: [r1] real;
writeln(r1.chpl_boundsCheck(r2)); // should print false
writeln(A[r2]);                   // should halt
