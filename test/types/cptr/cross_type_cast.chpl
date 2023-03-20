use CTypes;

var x : c_int = 1;
var xp : c_ptr(c_int) = c_ptrTo(x);
var xp_char = xp : c_ptr(c_char);
writeln(xp_char.deref());
