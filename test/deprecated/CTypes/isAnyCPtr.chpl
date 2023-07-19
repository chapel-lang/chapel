// deprecated by Jade in 1.31
use CTypes;
var x = 17;
var ptr = c_ptrTo(x);
var constPtr = c_ptrToConst(x);
var voidPtr = ptr:c_ptr(void);
// test all overloads
writeln(ptr.type:string,      " ", isAnyCPtr(ptr.type));
writeln(constPtr.type:string, " ", isAnyCPtr(constPtr.type));
writeln(voidPtr.type:string,  " ", isAnyCPtr(voidPtr.type));
writeln(x.type:string,        " ", isAnyCPtr(x.type));
