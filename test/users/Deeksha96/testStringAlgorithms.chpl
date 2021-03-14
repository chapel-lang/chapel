use StringAlgorithms;
var s_min1: string = minLexographicRotation("BCABDADAB");
var s_max1: string = maxLexographicRotation("BCABDADAB");
assert(s_min1 == "ABBCABDAD");
assert(s_max1 == "DADABBCAB");

var s_min2: string = minLexographicRotation("chapel");
var s_max2: string = maxLexographicRotation("chapel");
assert(s_min2 == "apelch");
assert(s_max2 == "pelcha");
