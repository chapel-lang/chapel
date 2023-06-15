// Initialization of `range(bool)` from `range(enum)`
// or range(u/int(?)) should be disallowed.

// When this is fixed, uncomment these cases in illegal-inits.compopts
// and remove this test.

var i: range(uint);
var b: range(bool) = i;    // illegal initialization

enum color { red };

var c1: range(color) = i;  // illegal initialization
var c2: range(color) = b;  // illegal initialization
var i1: range(int)  = c1;  // illegal initialization
var i2: range(uint) = c2;  // illegal initialization

compilerError("OK");       // should not happen
