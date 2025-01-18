use CTypes;
var s = "foo bar";
var cptr = "foo bar".c_str();

assert(s.type == string);
assert(cptr.type == c_ptrConst(c_char));
