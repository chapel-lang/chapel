/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Ben Harshbarger and Brad Chamberlain
   derived from the Rust #2 version by Matt Brubeck
*/
use IO;

const table = initTable("ATCGGCTAUAMKRYWWSSYRKMVBHDDHBVNN\n\n");

proc main(args: [] string) {
  const stdin = openfd(0),
        input = stdin.reader(iokind.native, locking=false,
                             hints=QIO_HINT_PARALLEL),
        len = stdin.size;
  var data: [0..#len] uint(8);

  // if the file isn't empty, wait for all tasks to complete before continuing
  if len then sync {
    do {
      // Mark where we start scanning and capture the starting offset
      const descOffset = input.mark();

      // Scan forward until we get to '\n' (end of description)
      input.advancePastByte("\n".toByte());
      const seqOffset = input.offset();

      // Scan forward until we get to '>' (end of sequence) or EOF
      const (eof, nextDescOffset) = findNextDesc();

      // look for the next description, returning '(eof, its offset)'
      proc findNextDesc() throws {
        try {
          input.advancePastByte(">".toByte());
        } catch (e:EOFError) {
          return (true, len-1);
        }
        return (false, input.offset()-1);
      }

      // Go back to the point we marked
      input.revert();

      // Read up to the nextDescOffset into the data array.
      input.read(data[descOffset..nextDescOffset]);

      // chars to rewind past: 1 for '\n' and 1 for '>' if we're not yet at eof
      const rewind = if eof then 1 else 2;

      // fire off a task to process the data for this sequence
      begin process(data[seqOffset..nextDescOffset-rewind]);
    } while !eof;
  }

  // write the data out to stdout once all tasks have completed
  const stdoutBin = openfd(1).writer(iokind.native, locking=false,
                                     hints=QIO_CH_ALWAYS_UNBUFFERED);
  stdoutBin.write(data);
}

// process a sequence from both ends, replacing each extreme element
// with the table lookup of the opposite one
proc process(seq: [?inds]) {
  var start = inds.low, end = inds.high;
  while start <= end {
    ref d1 = seq[start], d2 = seq[end];
    (d1, d2) = (table[d2], table[d1]);
    advance(start, 1);
    advance(end, -1);
  }

  proc advance(ref cursor, dir) {
    do { cursor += dir; } while seq[cursor] == "\n".toByte();
  }
}

proc initTable(pairs) {
  var table: [1..128] uint(8);

  for i in 1..pairs.numBytes by 2 {
    table[pairs.byte(i)] = pairs.byte(i+1);
    if pairs.byte(i) != "\n".toByte() then
      table[pairs[i:byteIndex].toLower().toByte()] = pairs.byte(i+1);
  }

  return table;
}
