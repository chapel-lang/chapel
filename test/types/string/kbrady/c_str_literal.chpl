use CTypes;
var cs = c"foo bar";
var s = "foo bar";
var cptr = "foo bar".c_str();

assert(cs.type == c_string);
assert(s.type == string);
assert(cptr.type == c_ptrConst(c_char));
