use CTypes;

var d : domain(1) = {1..10};

var ptr : c_ptr(domain(1)) = c_addrOf(d);
var constPtr : c_ptrConst(domain(1)) = c_addrOfConst(d);
