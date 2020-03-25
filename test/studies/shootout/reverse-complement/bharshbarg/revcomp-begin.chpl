/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Ben Harshbarger
   derived from the GNU C version by Jeremy Zerfas
*/
use IO;

var table : [1..128] uint(8);

const newLine     = "\n".toByte();
const greaterThan = ">".toByte();

proc main(args: [] string) {
  var inFile = openfd(0);
  const fileLen = inFile.size;
  var data : [1..fileLen] uint(8);
  var r = inFile.reader(locking=false);

  const pairs = [c in "ATCGGCTAUAMKRYWWSSYRKMVBHDDHBVNN\n\n".bytes()] c;

  // initialize complement table
  for i in 1..pairs.size by 2 {
    table[pairs[i]] = pairs[i+1];      // uppercase
    if pairs[i] != newLine then
      table[pairs[i] + 32] = pairs[i+1]; // lowercase
  }

  var numRead  : int;
  var idx = 1;
  var start = 0;

  // sync statements wait for all tasks inside them to complete
  sync {
    while r.readline(data, numRead, idx) {

      // Look for the start of a section, and if possible 
      // spawn a task to start work on the previous section.
      if data[idx] == greaterThan {
        if start == 0 then start = idx;
        else {
          begin process(data, start, idx-2);
          start = idx;
        }
      }
      idx += numRead; 
    }

    // work on the last section
    process(data, start, idx-2);
  }

  // Open a binary writer to stdout
  var binout = openfd(1).writer(iokind.native, locking=false, 
                                hints=QIO_CH_ALWAYS_UNBUFFERED);
  binout.write(data);
}

proc process(data : [], in front : int, in back : int) {

  // Advance to the next line
  while data[front] != newLine do front += 1;
  front += 1;

  // Walk backward past any newlines
  while data[back] == newLine do back -= 1;

  while front <= back {
    const temp  = table[data[front]];
    data[front] = table[data[back]];
    data[back]  = temp;

    // Advance to next character, skip any newlines
    do { front += 1; } while data[front] == newLine;
    do { back  -= 1; } while data[back]  == newLine;
  }
}
