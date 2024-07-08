
use CTypes;

var A:[0..10] int;

var eltZero = c_ptrToConst(A[0]);
var eltOne = c_ptrToConst(A[1]);

var diff = eltOne - eltZero;
writeln(diff);
assert(isIntType(diff.type));
assert(c_sizeof(diff.type) == c_sizeof(c_ptrdiff));

diff = eltOne - eltOne;
writeln(diff);
