// Regression test for https://github.com/chapel-lang/chapel/issues/16015.

var A:[1..2, 3..4] int;
ref B = A[1..2, 3]; // rank change

proc f(ref X: [?D] int, ref Y: [D] int) { writeln(X, " | ", Y); }

f(B, B);
