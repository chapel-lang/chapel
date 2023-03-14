use CTypes;

var x : c_int = 5;
var xp_void : c_void_ptr = c_ptrTo(x);
writeln(xp_void);
var xp = xp_void : c_ptrConst(c_int);
writeln(xp_void);
var xp_voidAgain : c_void_ptr = xp : c_void_ptr;
writeln(xp_void);

