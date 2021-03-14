use Vector;
use List;
var l = new list(1..9);
var a = [1,2,3,4,5,6,7,8,9];
var v1 = new vector(l);
var v2 = new vector(v1);
var v3 = new vector(1..9);
var v4 = new vector(a);

writeln(v1);
writeln(v2);
writeln(v3);
writeln(v4);
