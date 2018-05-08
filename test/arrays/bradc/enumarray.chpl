enum classVals { S, W, A, B, C, D, O };

var DClass: domain(1) = {classVals.S:int..classVals.O:int};

var probSize: [DClass] int;

probSize(classVals.S:int) = 123;
probSize(classVals.W:int) = 456;
probSize(classVals.A:int) = 789;
probSize(classVals.B:int) = 012;
probSize(classVals.C:int) = 345;
probSize(classVals.D:int) = 678;
probSize(classVals.O:int) = 901;

writeln("probSize(S) = ", probSize(classVals.S:int));
for i in DClass {
  writeln("probSize(", i:classVals, ") = ", probSize(i));
}
writeln("probSize = ", probSize);
