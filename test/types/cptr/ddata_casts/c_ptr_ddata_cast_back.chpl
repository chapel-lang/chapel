use CTypes;

var a : int = 4;

// casting from matching element type: ok
var mycptr : c_ptr(int) = c_ptrTo(a);
var myddata : _ddata(int) = mycptr : _ddata(int);
// casting back to c_ptr, even with matching element type: not ok
var mycptragain : c_ptr(int) = myddata : c_ptr(int);
