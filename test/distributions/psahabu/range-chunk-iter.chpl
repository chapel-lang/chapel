use RangeChunk;

config param cnks = 5;

const pos  = 0..100;
const neg = -100..0;
const rev = 0..100 by -1;
const non = 100..0;

writeln("positive:");
for chunk in chunks(pos, 5) do
  writeln(chunk);

writeln("negative:");
for chunk in chunks(neg, 5) do
  writeln(chunk);

writeln("reverse:");
for chunk in chunks(rev, 5) do
  writeln(chunk);

writeln("empty:");
for chunk in chunks(non, 5) do
  writeln("this should never print");
