use BlockDist;

var Space = {1..10};
const D = Space dmapped Block(boundingBox=Space);
var A: [D] real = [i in D] i;

var localD = D.localSubdomain();
var localR = localD.low .. localD.high;
var subR = localR.low+1 .. localR.high;

writeln(A.localSlice(localD by 2));
writeln(A.localSlice(localR by 2));

var B: [D] real = [i in D] i;

writeln(B.localSlice(localD by 2));

A.localSlice({localR by 2}) = -1;
A.localSlice({subR by 2}) = -2;

B.localSlice(localR by 2) = -1;
B.localSlice(subR by 2) = -2;

writeln("A is: ", A);
writeln("B is: ", B);

