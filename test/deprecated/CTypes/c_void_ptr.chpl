// deprecated in 1.32

use CTypes;

var a : int = 4;
var ptr : c_ptr(int) = c_ptrTo(a);
var ptr_void_old : c_void_ptr = ptr : c_void_ptr;   // deprecated
var ptr_void_new : c_ptr(void) = ptr : c_ptr(void); // replacement
