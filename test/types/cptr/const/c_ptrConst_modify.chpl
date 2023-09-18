use CTypes;

var x : c_int = 5;
var xp : c_ptrConst(c_int) = c_ptrToConst(x);
writeln(xp.deref());
xp.deref() = 4;
