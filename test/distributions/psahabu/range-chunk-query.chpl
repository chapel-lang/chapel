use RangeChunk;

config param cnks = 5;

const pos  = 0..100;
const neg = -100..0;
const rev = 0..100 by -1;
const non = 100..0;

writeln("positive:");
writeln(chunk(pos, cnks, 0));
writeln(chunk(pos, cnks, 2));
writeln(chunk(pos, cnks, 4));

writeln("negative:");
writeln(chunk(neg, cnks, 0));
writeln(chunk(neg, cnks, 2));
writeln(chunk(neg, cnks, 4));

writeln("reverse:");
writeln(chunk(rev, cnks, 0));
writeln(chunk(rev, cnks, 2));
writeln(chunk(rev, cnks, 4));

writeln("out of bounds");
writeln(chunk(pos, cnks, -1));
writeln(chunk(pos, cnks, 5));
