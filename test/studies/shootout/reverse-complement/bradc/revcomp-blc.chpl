/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/
   contributed by Ben Harshbarger
   derived from the Rust #2 version by Matt Brubeck
*/

const table = initTable("ATCGGCTAUAMKRYWWSSYRKMVBHDDHBVNN\n\n");

config const readSize = 16 * 1024;

proc main(args: [] string) {
  const stdin = openfd(0),
        input = stdin.reader(iokind.native, locking=false),
        len = stdin.length();
  var data : [0..#len] uint(8);

  sync { // wait for all process() tasks to complete before continuing
    if len {  // make sure the file is not empty
      do {
        const descOffset = input._offset();

        // Mark where we start scanning (keep bytes in I/O buffer in input)
        input._mark();

        // Scan forward until we get to '\n' (end of description)
        input.advancePastByte(ascii("\n"));
        const seqOffset = input._offset();

        // Scan forward until we get to '>' (end of sequence) or EOF
        const (eof, nextDescOffset) = findNextDesc();

        // look for the next description, returning '(eof, its offset)'
        proc findNextDesc() throws {
          try {
            input.advancePastByte(ascii(">"));
          } catch (e:EOFError) {
            return (true, len);
          }
          return (false, input._offset());
        }

        // Go back to the point we marked
        input._revert();

        // Read until nextDescOffset into the data array.
        input.readBytes(data[descOffset], nextDescOffset-descOffset);

        // '2' to skip over '\n' + 1 to skip over '>' if not yet at eof
        begin process(data, seqOffset, nextDescOffset-(2+!eof));
      } while !eof;
    }
  }

  const stdoutBin = openfd(1).writer(iokind.native, locking=false,
                                     hints=QIO_CH_ALWAYS_UNBUFFERED);
  stdoutBin.write(data);
}

proc process(data, in start, in end) {
  while start <= end {
    ref d1 = data[start], d2 = data[end];
    (d1, d2) = (table[d2], table[d1]);
    advance(start, 1);
    advance(end, -1);
  }

  proc advance(ref cursor, dir) {
    do { cursor += dir; } while data[cursor] == ascii("\n");
  }
}

proc initTable(pairs) {
  var table: [1..128] uint(8);

  for i in 1..pairs.length by 2 {
    table[ascii(pairs[i])] = ascii(pairs[i+1]);
    if pairs[i] != "\n" then
      table[ascii(pairs[i].toLower())] = ascii(pairs[i+1]);
  }

  return table;
}
