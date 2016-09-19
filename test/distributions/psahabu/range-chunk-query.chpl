var pos  = 0..100;
var neg = -100..0;
var rev = 0..100 by -1;

writeln("positive:");
writeln(chunk(pos, 5, 0));
writeln(chunk(pos, 5, 2));
writeln(chunk(pos, 5, 4));

writeln("negative:");
writeln(chunk(neg, 5, 0));
writeln(chunk(neg, 5, 2));
writeln(chunk(neg, 5, 4));

writeln("reverse:");
writeln(chunk(rev, 5, 0));
writeln(chunk(rev, 5, 2));
writeln(chunk(rev, 5, 4));
