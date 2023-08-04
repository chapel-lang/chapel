use CTypes;
proc f() { }

var ptr = c_ptrTo(f):c_ptr(void);
writeln(ptr.type:string);
