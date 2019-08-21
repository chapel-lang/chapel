var D0: domain(int);
var D1: domain(int);
var D2: domain(int);
var A0: [D0] int;
var A1: [D1] int;
var A2: [D2] int;

D0 += 1; D0 += 2; D0 += 3;
D1 = D0;
D2 += 1; D2 += 3; D2 += 5;

A0[1] = 1; A0[2] = 2; A0[3] = 3;
A1 = A0;
A2[1] = -1; A2[3] = -3; A2[5] = -5;

A1 += A2; writeln(A1.sorted()); D1 = D0; A1 = A0;
A1 |= A2; writeln(A1.sorted()); D1 = D0; A1 = A0;
A1 &= A2; writeln(A1.sorted()); D1 = D0; A1 = A0;
A1 -= A2; writeln(A1.sorted()); D1 = D0; A1 = A0;
A1 ^= A2; writeln(A1.sorted()); D1 = D0; A1 = A0;
