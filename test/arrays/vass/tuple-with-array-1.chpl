
type Q = [1..1] borrowed object?;

// this currently works
var x0: Q;
writeln(x0);

// this currently doesn't
var x1: 1*Q;
writeln(x1);

// more of the same
var x1i: 1*Q = (x0,);
var x2: (Q,Q);
var x3: (int, Q);
var x4: (Q, int);
writeln(x1i, x2, x3, x4);
