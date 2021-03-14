use RangeChunk;
use RemElems;

var r = 1..12;

for rp in (Thru, Pack, Mod) {
  writeln(rp);
  for (chunk, i) in zip(chunks(r, 8, remPol = rp), 0..) do
    // index: chunk (numelems)
    writeln(i, ": ", chunk, " (", chunk.high-chunk.low+1, ")");
  writeln();
}
