/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Ben Harshbarger and Brad Chamberlain
   derived from the GNU C version by Mr Ledrug
*/
use IO;

config param columns = 61;

const table = initTable("ATCGGCTAUAMKRYWWSSYRKMVBHDDHBVNN\n\n");

proc main(args: [] string) {
  const consoleIn = openfd(0),
        stdinNoLock = consoleIn.reader(locking=false);

  var data: [1..consoleIn.size] uint(8),
      idx = 1,
      start = 0;

  sync {     // wait for all process() tasks to complete before continuing
    var numRead: int;

    while stdinNoLock.readline(data, numRead, idx) {
      if data[idx] == ">".toByte() {       // is this the start of a section?

        // spawn a task to process the previous sequence, if there was one
        if start then
          begin process(data, start, idx-2);     // -2 == rewind past "\n>"

        // capture the start of this sequence
        start = idx + numRead;
      }

      idx += numRead; 
    }

    // process the final sequence
    if start then
      process(data, start, idx-2);
  }

  const stdoutBin = openfd(1).writer(iokind.native, locking=false, 
                                     hints=QIO_CH_ALWAYS_UNBUFFERED);
  stdoutBin.write(data);
}


proc process(data, start, end) {
  const extra = (end - start) % columns,
        off = columns - extra - 1;

  // shift the data
  if off then
    for m in (start+extra)..(end-1) by columns {
      for i in 1..off-1 by -1 do
        data[m+i+1] = data[m+i];
      data[m+1] = "\n".toByte();
    }

  // replace the data items with their table entries
  for i in 0..(end-start)/2 {
    ref d1 = data[start+i],
        d2 = data[end-i];
    (d1, d2) = (table[d2], table[d1]);
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
