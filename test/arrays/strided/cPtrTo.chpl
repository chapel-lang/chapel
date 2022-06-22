use CTypes;

var A: [1..10 by 2 align 2] real;

var Astart = c_ptrTo(A);
var A2 = c_ptrTo(A[2]);

writeln(Astart == A2);

