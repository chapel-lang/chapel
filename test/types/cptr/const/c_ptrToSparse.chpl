use CTypes;

var D = {1..3};
var DS: sparse subdomain(D) = {2};

var A:[DS] real;

var aptr_const = c_ptrToConst(A);
