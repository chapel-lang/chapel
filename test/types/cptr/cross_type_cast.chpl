use CTypes;

var x : c_int = 1;
var xp : c_ptr(c_int) = c_ptrTo(x);

// acceptable casts for strict aliasing
var xp_char = xp : c_ptr(c_char);
var xp_schar = xp : c_ptr(c_schar);
var xp_uchar = xp : c_ptr(c_uchar);
var xp_uint = xp : c_ptr(c_uint);

// unacceptable casts for strict aliasing
var xp_float = xp : c_ptr(c_float);
var xp_double = xp : c_ptr(c_double);

writeln(xp_char.deref());
