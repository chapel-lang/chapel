var binL = 0b111;
var binS = "0b111": uint;

var octL = 0o777;
var octS = "0o777": uint;

var decL = 999;
var decS = "999": uint;

var hexL = 0xffffffffffffffff;
var hexS = "0xffffffffffffffff":uint;

assert(binL == binS);
assert(octL == octS);
assert(decL == decS);
assert(hexL == hexS);
