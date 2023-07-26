use CTypes;

var a : int = 4;

// casting from non-matching element type: not ok
var mycptr : c_ptr(int) = c_ptrTo(a);
var myddata : _ddata(real) = mycptr : _ddata(real);
