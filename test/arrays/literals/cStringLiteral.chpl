use CTypes;
var A = [c"hi", c"there", c"everybody"];
var B = ["hi".c_str(), "there".c_str(), "everybody".c_str()];

assert(A.eltType == c_string);
assert(B.eltType == c_ptrConst(c_char));
