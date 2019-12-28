/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Brad Chamberlain
   based on the Chapel #1-#3 versions by Ben Harshbarger and myself,
   and informed by the C gcc #6 version by Jeremy Zerfas, particularly in
   terms of the read chunk size and the recursive doubling of the buffer
*/

// TODO: if I add a flush and remove the unbuffered, does perf improve?

use IO;

config const readSize = 16384;  // the chunk size to read at a time

param eol = "\n".toByte();      // end-of-line, as an integer

const table = createTable();    // create the table of code complements

// a channel and coordination variable for writing data to stdout
var stdoutBin = openfd(1).writer(iokind.native, locking=false,
                                 hints=QIO_CH_ALWAYS_UNBUFFERED),
    seqToWrite: atomic int = 1;


proc main(args: [] string) {
  const stdin = openfd(0),
        input = stdin.reader(iokind.native, locking=false);

  var curSeq = new Seq(id=1);

  do {
    const eof = curSeq.readChunk(input);

    // look for one or more sequences in the chunk we just read
    do {
      const foundSeq = curSeq.processChunk();

      if (foundSeq) {
        // if the chunk completed a sequence, save it away and copy the
        // remainder into the next one
        var lastSeq = curSeq;
        curSeq = lastSeq.copyTail();

        // then fire off an asynchronous task to process the last sequence
        begin with (in lastSeq) {
          lastSeq.revcomp();
        }
      }
    } while foundSeq == 1;
  } while !eof;
}

// a class representing a sequence
class Seq {
  const id = 0;              // the sequence ID
  var cursor = 0,            // a cursor for reading the next chunk
      last = 0,              // the index of the last byte in the chunk
      start = -1,            // the start of the sequence
      end = -1,              // the end of the sequence
      dataLen = readSize,    // the size of the data buffer
      inds = {0..#dataLen},  // the domain describing the data buffer
      data: [inds] uint(8);  // the data buffer

  // read a chunk of bytes into the data buffer from the input channel;
  // returns whether we've seen EOF (true) or not (false)
  proc readChunk(input) {
    last = cursor + readSize - 1;
    if (last >= dataLen) {
      dataLen *= 2;
      inds = {0..#dataLen};
    }
    return !input.read(data[cursor..last]);
  }

  // process the chunk from cursor through last looking for '>'s or '\0's;
  // returns 0 if we did not find the end of the sequence, 1 if we did and
  // another sequence follows, 2 if we did and it's the end of the file (EOF)
  proc processChunk() {
    for i in cursor..last {
      if data[i] == ">".toByte() {  // if we find '>'
        if start == -1 {            // and haven't started a sequence yet
          start = i;                // this is the start
        } else {
          end = i-1;                // otherwise, it starts the next one
          return 1;
        }
      } else if data[i] == 0 {      // if we find '\0', this is the file's end;
        end = i-1;
        if start != -1 then
          return 2;                 // indicate that we found a sequence
      }
    }

    // we did not find the end of a sequence, so advance the cursor to
    // prepare for the next read
    cursor = last+1;
    return 0;
  }

  // copy the unused tail of our data buffer into a new sequence
  proc copyTail() {
    const tailLen = last - end;
    var newSeq = new Seq(id = id+1, cursor = 0, last = tailLen-1);
    newSeq.data[0..#tailLen] = data[end+1..#tailLen];

    return newSeq;
  }

  // reverse and complement the sequence, then write out the result
  proc revcomp() {
    last = end;                  // snapshot the end of the sequence

    // advance 'start' to the first end-of-line
    do {
      start += 1;
    } while data[start] != eol;

    while start < end {
      // swap the end values, replacing them with their complements
      ref d1 = data[start],
          d2 = data[end];

      (d1, d2) = (table[d2], table[d1]);

      // advance the cursors, skipping over end-of-line characters
      advance(start, 1);
      advance(end, -1);

      proc advance(ref ind, dir) {
        do {
          ind += dir;
        } while data[ind] == eol;
      }
    }

    // write the sequence to stdout
    seqToWrite.waitFor(id);         // wait for our ID's turn to write
    stdoutBin.write(data[..last]);  // write the transformed data
    seqToWrite.write(id+1);         // make it the next sequence's turn
  }
}


proc createTable() {
  // `pairs` compactly represents the table we're creating, where the
  // first byte of each pair (in either case) maps to the second:
  //   A|a -> T, C|c -> G, G|g -> C, T|t -> A, etc.
  param pairs = b"ATCGGCTAUAMKRYWWSSYRKMVBHDDHBVNN",
        upperToLower = "a".toByte() - "A".toByte();

  var table: [0..127] uint(8);

  table[eol] = eol;
  for i in 1..pairs.size by 2 {
    const src = pairs.byte[i],
          dst = pairs.byte[i+1];

    table[src] = dst;
    table[src+upperToLower] = dst;
  }

  return table;
}
