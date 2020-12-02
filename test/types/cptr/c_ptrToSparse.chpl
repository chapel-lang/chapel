use CPtr;

var D = {1..3};
var DS: sparse subdomain(D) = {2};

var A:[DS] real;

var aptr = c_ptrTo(A);
