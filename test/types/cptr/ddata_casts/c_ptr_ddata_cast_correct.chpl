use CTypes;

var a : int = 4;

// casting from matching element type
var mycptr : c_ptr(int) = c_ptrTo(a);
var myddata : _ddata(int) = mycptr : _ddata(int);

writeln(myddata[0]);

// casting to and from void pointer
var mycptrvoid : c_ptr(void) = mycptr : c_ptr(void);
myddata = mycptrvoid : _ddata(int);
var mycptragain : c_ptr(int) = myddata : c_ptr(void) : c_ptr(int);

writeln(mycptragain.deref());
