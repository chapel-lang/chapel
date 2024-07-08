use CTypes;

var a : int = 4;

// casting from non-matching element type: ok
// Unsure if this is intentional. This test was added while ensuring the
// c_void_ptr->c_ptr(void) replacement didn't affect anything, and this was
// also possible before. Anna, July 2023
var mycptr : c_ptr(int) = c_ptrTo(a);
var myddata : _ddata(real) = mycptr : _ddata(real);
