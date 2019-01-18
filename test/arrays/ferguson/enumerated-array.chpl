enum classVals {S, W, A, B, C, D};

const Class: domain(classVals) = classVals.S..classVals.D;
var   probSizes:   [Class] int = [classVals.S=>1400, classVals.W=>7000, classVals.A=>14000, classVals.B=>75000, classVals.C=>150000, classVals.D=>150000];

ref x = probSizes(classVals.S);
x += 1;
writeln("x ", x);
assert(x == 1401);
var y = probSizes(classVals.W);
writeln("y ", y);
assert(y == 7000);

