// Illegal initializations should produce a user-facing error message:
//   range(bool) from range(uint)
//   range(enum) from range(uint)
//   range(int(8)) from range(int) and similar

// When this is fixed, uncomment these cases in illegal-inits.compopts
// and remove this test.

enum color { red };

var u: range(uint);
var c: range(color) = u; // illegal initialization

compilerError("OK");    // should not happen
