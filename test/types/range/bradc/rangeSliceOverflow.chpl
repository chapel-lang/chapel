var n = 2**17;

var r1 = 0..2*n by n;
var r2 = 0..2*n by n;

var r3 = r1[r2];

writeln("r3 is: ", r3);

