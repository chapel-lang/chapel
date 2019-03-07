use LinearAlgebra;

// Tests to confirm promotion flattening is not an issue
var A = Matrix(4, 4),
    B = Matrix(4, 4),
    C = Matrix(4, 2);

var PLUS = A + B;
assert(PLUS.shape == A.shape);

var PLUS2 = A + 2;
assert(PLUS2.shape == A.shape);

var MINUS = A - B;
assert(MINUS.shape == A.shape);

var MINUS2 = A - 2;
assert(MINUS2.shape == A.shape);

var TIMES = A * B;
assert(TIMES.shape == A.shape);

var TIMES2 = A * 2;
assert(TIMES2.shape == A.shape);

var DIV2 = A / 2;
assert(DIV2.shape == A.shape);

var MULT = A.dot(C);
assert(MULT.shape == C.shape);
