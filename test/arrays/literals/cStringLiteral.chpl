use CTypes;
var B = ["hi".c_str(), "there".c_str(), "everybody".c_str()];

assert(B.eltType == c_ptrConst(c_char));
