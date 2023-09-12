use BlockDist;

var a: [1..10] int = 10;
var c = blockDist.createArray(0..10, int);
c = 199;

var b = a.tryCopy();
var d = c.tryCopy();

writeln(b);
writeln(d);
