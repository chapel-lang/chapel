use RangeChunk;

var pos  = 0..100;
var neg = -100..0;
var rev = 0..100 by -1;

writeln("positive:");
for chunk in chunks(pos, 5) do
  writeln("(" + chunk.first + ", " + chunk.last + ")");

writeln("negative:");
for chunk in chunks(neg, 5) do
  writeln("(" + chunk.first + ", " + chunk.last + ")");

writeln("reverse:");
for chunk in chunks(rev, 5) do
  writeln("(" + chunk.first + ", " + chunk.last + ")");
