use SysCTypes;
use CPtr;

var a: c_void_ptr = (-1): c_void_ptr;
var b: c_void_ptr = (-1): int : c_void_ptr;
var c: c_void_ptr = (-1): uint : c_void_ptr;
var d: c_void_ptr = (-1): c_intptr : c_void_ptr;
var e: c_void_ptr = (-1): c_uintptr : c_void_ptr;

assert(a == b);
assert(b == c);
assert(c == d);
assert(d == e);

var x = a: c_intptr;
assert(x == -1);
