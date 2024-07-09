var r1: range(int(32), boundKind.both, strideKind.one);
var r2: range(int, boundKind.both);

r1 = 1..4:int(32);
r2 = 1..4;

var D = {r1,r1,r2};
writeln(D);
