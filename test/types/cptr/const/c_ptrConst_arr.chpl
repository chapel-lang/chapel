use CTypes;

var x : c_int = 5;
var xp : c_ptrConst(c_int) = c_ptrToConst(x);
writeln(xp.deref());


const y = [1, 2, 3];
var yp : c_ptrConst(int) = c_ptrToConst(y);
writeln(yp[0], yp[1], yp[2]);
