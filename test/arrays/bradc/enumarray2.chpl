enum classVals { S, W, A, B, C, D, O };

var DClass: domain(classVals) = [S..O];

var probSize: [DClass] int;

probSize(S) = 123;
probSize(W) = 456;
probSize(A) = 789;
probSize(B) = 012;
probSize(C) = 345;
probSize(D) = 678;
probSize(O) = 901;

writeln("probSize(S) = ", probSize(S));
for i in DClass {
  writeln("probSize(", i, ") = ", probSize(i));
}
writeln("probSize = ", probSize);
