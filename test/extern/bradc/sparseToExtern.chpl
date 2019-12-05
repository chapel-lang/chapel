use SysCTypes;

extern proc cprintarr(X: [] real, n: c_int);

var D = {1..3};
var DS: sparse subdomain(D) = {2};

var A:[DS] real;

A[2] = 1.2;

cprintarr(A, 1: c_int);
