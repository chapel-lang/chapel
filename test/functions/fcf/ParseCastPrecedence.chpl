// This should parse the way we expect it to, the first colon is not
// a cast. Is this possible, though? Or do we just have to require
// parens?
var x1 = proc(ref x: real): int:string == "proc(ref x: real): int";
assert(x1);

var x2 = proc(ref x: real): int:string:string == "proc(ref x: real): int";
assert(x2);

