// This should generate a clear error message.

extern type LAPACK_C_SELECT1 ;
var x: LAPACK_C_SELECT1;
writeln(x);

proc foo(arg: LAPACK_C_SELECT1) { } // OK; what is the argument intent??
foo(x);
