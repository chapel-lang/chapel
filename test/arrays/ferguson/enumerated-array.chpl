enum classVals {S, W, A, B, C, D};

const Class: domain(classVals);
const probSizes:   [Class] int = ( 1400, 7000, 14000, 75000, 150000, 150000 );

ref x = probSizes(classVals.S);
x += 1;
writeln("x ", x);
assert(x == 1401);
var y = probSizes(classVals.W);
writeln("y ", y);
assert(y == 7000);

