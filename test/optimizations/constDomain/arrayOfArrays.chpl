config const n = 10;

var A: [1..n][1..3]real;

writeln(A.domain._value.definedConst);
writeln(A.domain.definedConst);

writeln(A[1].domain._value.definedConst);
writeln(A[1].domain.definedConst);
