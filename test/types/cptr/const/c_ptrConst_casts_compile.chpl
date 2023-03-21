use CTypes;

var x = 5;
var xp : c_ptrConst(int) = c_ptrToConst(x);


var a = xp : c_ptrConst(real);
var b = xp : c_ptr(int);
var c = xp : string;
var d = xp : c_void_ptr;
var e = xp : c_intptr;
