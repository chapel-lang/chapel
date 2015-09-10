use "void_extern_proc_array.h";

extern proc voidArrayFunction(array: [] c_int, elems : c_int): void;

var array: [1..10] c_int;

voidArrayFunction( array, array.domain.size : c_int );
