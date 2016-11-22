use RangeChunk;
use RemElems;

var r = 1..10;
for rp in (Thru, Pack, Mod) {
  writeln(rp);
  for (chunk, i) in zip(chunks(r, 15, rp), 0..) do
    writeln(i, ": ", chunk, " (", chunk.high-chunk.low+1, ")");
  writeln();
}


