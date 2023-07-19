use CTypes;

var a : int = 4;
var ptr = c_ptrTo(a);
var void_ptr = ptr : c_ptr(void);
writeln(void_ptr.deref());
