use Math;

var c = 1.0+2.0i;
var d = 2.0+3.0i;
const val = (c*conj(d)).re;
writeln(val: int); // Cast to int for more reliable output
