use CPtr;
proc f() { }

var ptr = c_ptrTo(f):c_void_ptr;
writeln(ptr.type:string);
