use BlockDist;

const size = 10;
const D = {0..#size} dmapped new blockDist(boundingBox={0..#size});
var a: [D] int = 1;
var b: [D] int = 2;

a <=> b;

writeln(a);
writeln(b);
