/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/
   contributed by Ben Harshbarger
   derived from the Rust #2 version by Matt Brubeck
*/
use IO, SysCTypes;

const table = initTable("ATCGGCTAUAMKRYWWSSYRKMVBHDDHBVNN\n\n");

config const readSize = 16 * 1024;

proc main(args: [] string) {
  const stdin = openfd(0);
  var input = stdin.reader(iokind.native, locking=false,
                           hints=QIO_HINT_PARALLEL);
  var len = stdin.size;
  var data : [0..#len] uint(8);
  
  sync { // wait for all process() tasks to complete before continuing

    while true {
      const descOffset = input.offset();
      var nextDescOffset = descOffset;
      var seqOffset = descOffset;
      var eof = false;

      // Mark where we start scanning (keep bytes in I/O buffer in input)
      input.mark();

      // Scan forward until we get to the \n (end of description)
      input.advancePastByte("\n".toByte());
      seqOffset = input.offset();

      try {
        // Scan forward until we get to the > (end of sequence)
        input.advancePastByte(">".toByte());
        nextDescOffset = input.offset();
      } catch e:EOFError {
        eof = true;
        nextDescOffset = len;
      }

      // Go back to the point we marked
      input.revert();

      // Read until nextDescOffset into the data array.
      input.readBytes(c_ptrTo(data[descOffset]),
          (nextDescOffset-descOffset):ssize_t);
      

      if !eof {
        // '-3' to skip over '\n>'
        begin process(data, seqOffset, nextDescOffset-3);
      } else {
        // '-2' to skip over '\n'
        begin process(data, seqOffset, len-2);
        break;
      }
    }
  }

  const stdoutBin = openfd(1).writer(iokind.native, locking=false, 
                                     hints=QIO_CH_ALWAYS_UNBUFFERED);
  stdoutBin.write(data);
}

proc process(data, in start, in end) {

  proc advance(ref cursor, dir) {
    do { cursor += dir; } while data[cursor] == "\n".toByte();
  }
  while start <= end {
    ref d1 = data[start], d2 = data[end];
    (d1, d2) = (table[d2], table[d1]);
    advance(start, 1);
    advance(end, -1);
  }
}

proc initTable(pairs) {
  var table: [1..128] uint(8);

  for i in 0..#pairs.numBytes by 2 {
    table[pairs.byte(i)] = pairs.byte(i+1);
    if pairs.byte(i) != "\n".toByte() then
      table[pairs[i:byteIndex].toLower().toByte()] = pairs.byte(i+1);
  }

  return table;
}
