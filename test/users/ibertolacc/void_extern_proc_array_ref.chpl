require "void_extern_proc_array.h", "void_extern_proc_array.c";

use CTypes;

extern proc voidArrayFunction(ref array: [] c_int, elems : c_int): void;

var array: [1..10] c_int;

voidArrayFunction( array, array.domain.size : c_int );
