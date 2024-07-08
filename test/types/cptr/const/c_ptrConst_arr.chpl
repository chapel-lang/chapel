use CTypes;

const y = [1, 2, 3];
var yp : c_ptrConst(int) = c_ptrToConst(y);
writeln(yp[0], yp[1], yp[2]);
