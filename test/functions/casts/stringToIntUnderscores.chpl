var binL = 0b11_1;
var binS = "0b11_1": int;

var octL = 0o7_77;
var octS = "0o7_77": int;

var decL = 99_9;
var decS = "99_9": int;

var hexL = 0xF_FF;
var hexS = "0xF_FF": int;

assert(binL == binS);
assert(octL == octS);
assert(decL == decS);
assert(hexL == hexS);
