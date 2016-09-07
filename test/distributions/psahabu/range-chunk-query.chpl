use RangeChunk;

var pos  = 0..100;
var neg = -100..0;
var rev = 0..100 by -1;

writeln("positive:");
writeln(chunk(pos, 5, 1));
writeln(chunk(pos, 5, 3));
writeln(chunk(pos, 5, 5));

writeln("negative:");
writeln(chunk(neg, 5, 1));
writeln(chunk(neg, 5, 3));
writeln(chunk(neg, 5, 5));

writeln("reverse:");
writeln(chunk(rev, 5, 1));
writeln(chunk(rev, 5, 3));
writeln(chunk(rev, 5, 5));
