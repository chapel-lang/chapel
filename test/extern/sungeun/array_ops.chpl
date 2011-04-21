_extern proc clear_int_array(inout A: int, len: int);
_extern proc copy_int_array(inout dest: int, inout src: int, len:int);

config const n = 10;

var A: [1..n] int;

A = [i in 1..n] min(int)+i;

clear_int_array(A(1), n);
writeln(A==0);

A = [i in 1..n] max(int)-i;

var B: [1..n] int;

copy_int_array(B(1), A(1), n);
writeln(A==B);
