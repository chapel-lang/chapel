use SysCTypes;

extern proc cprintarr(X: [] real, n: c_int);

var A: [1..3] real = [i in 1..3] i:real;

var B: [1..2, 1..2] real = [(i,j) in {1..2, 1..2}] i + j/2.0;

var C: [1..2, 1..2, 1..2] real = 
  [(i,j,k) in {1..2, 1..2, 1..2}] i*10 + j + k/2.0;

cprintarr(A, A.size:c_int);
cprintarr(B, B.size:c_int);
cprintarr(C, C.size:c_int);

