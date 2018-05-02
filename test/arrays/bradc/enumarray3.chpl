enum classVals { S, W, A, B, C, D, O };

var DClass = {classVals.S..classVals.O};

var probSize: [DClass] int;

probSize(classVals.S) = 123;
probSize(classVals.W) = 456;
probSize(classVals.A) = 789;
probSize(classVals.B) = 012;
probSize(classVals.C) = 345;
probSize(classVals.D) = 678;
probSize(classVals.O) = 901;

writeln("probSize(S) = ", probSize(classVals.S));
for i in DClass {
  writeln("probSize(", i:classVals, ") = ", probSize(i));
}
writeln("probSize = ", probSize);
