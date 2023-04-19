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


// same thing but for const pointers

var xpconst : c_ptrConst(c_int) = c_ptrToConst(x);

var xpconst_char = xpconst : c_ptrConst(c_char);
var xpconst_schar = xpconst : c_ptrConst(c_schar);
var xpconst_uchar = xpconst : c_ptrConst(c_uchar);
var xpconst_uint = xpconst : c_ptrConst(c_uint);

var xpconst_float = xpconst : c_ptrConst(c_float);
var xpconst_double = xpconst : c_ptrConst(c_double);

writeln(xpconst_char.deref());


// should also warn from non-const to const and vice-versa
var xpconst_float_from_nonconst = xp : c_ptrConst(c_float);
var xp_float_from_const = xpconst : c_ptr(c_float);


// double pointers should recursively check pointee types
var xpp : c_ptr(c_ptr(c_int)) = c_ptrTo(xp);
var xpp_char = xpp : c_ptr(c_ptr(c_int)); // acceptable to strict aliasing
var xpp_float = xpp : c_ptr(c_ptr(c_float)); // unacceptable

// casting to and from void pointer pointee type is ok
var xpp_void = xpp : c_ptr(c_void_ptr);
var xpp_void_int_again = xpp_void : c_ptr(c_ptr(c_int));
