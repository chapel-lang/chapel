use Memory.Initialization;

var n: nothing;
var A: [1..3] real;
var D = {1..3};
var dist = D.dist;
var i: int;
var r: real;

moveInitialize(n, i);
moveInitialize(A, i);
moveInitialize(D, i);
moveInitialize(dist, i);
moveInitialize(i, r);

writeln("Got to end");
