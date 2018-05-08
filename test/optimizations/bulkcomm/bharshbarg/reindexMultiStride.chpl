
//
// A test to ensure that we correctly translate the final slice, 1..20 by 4,
// to the original domain's indices: 1..9 by 2
//

var A : [1..10] int;
for i in 1..10 do A[i] = i;

ref X = A.reindex(1..20 by 2);
ref Y = X[1..20 by 4];

var T = [-1, -2, -3, -4, -5];
Y = T;
writeln(A);
