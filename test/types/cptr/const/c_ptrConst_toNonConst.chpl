use CTypes;

var x : c_int = 5;
var xpconst : c_ptrConst(c_int) = c_ptrToConst(x);
var xpnonconst : c_ptr(c_int) = xpconst : c_ptr(c_int);
writeln(xpnonconst.deref());
