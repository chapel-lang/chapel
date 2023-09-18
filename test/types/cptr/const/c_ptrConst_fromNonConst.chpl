use CTypes;

var x : c_int = 5;
var xp : c_ptr(c_int) = c_ptrTo(x);
var xpconst : c_ptrConst(c_int) = xp : c_ptrConst(c_int);
writeln(xpconst.deref());
