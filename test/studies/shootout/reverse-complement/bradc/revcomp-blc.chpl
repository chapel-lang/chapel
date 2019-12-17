/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Ben Harshbarger and Brad Chamberlain
   derived from the Rust #2 version by Matt Brubeck
   updated to avoid allocating the buffer to be the right size from the start
*/

// TODO: All variables read?

use IO;

config const debug = false;

config const readSize = 16384;

param eol = "\n".toByte();    // end-of-line, as an integer

const table = createTable();  // create the table of code complements

// write the data that we read to stdout once all tasks have completed	
const stdoutBin = openfd(1).writer(iokind.native, locking=false,
                                   hints=QIO_CH_ALWAYS_UNBUFFERED);
var seqToWrite: atomic int = 1;

class Seq {
  const id = 0;
  var cursor = 0,
      last = 0,
      start = -1,
      end = -1;
  var dataLen = readSize;
  var dom = {0..#dataLen};
  var data: [dom] uint(8);

  proc readChunk(input) {
    last = cursor + readSize - 1;
    if (last >= dataLen) {
      dataLen *= 2;
      if debug then writeln("resizing seq ", id, " to ", dataLen);
      dom = {0..#dataLen};
    }
    if debug then writeln("reading into ", cursor..last);
    return !input.read(data[cursor..last]);
  }

  proc processChunk() {
    for i in cursor..last {
      if data[i] == ">".toByte() {
        if debug then writeln("Found a '>' at offset ", i);
        if start == -1 {
          if debug then writeln("Setting start to ", i);  // TODO: we always set start to 0
          start = i;
	} else {
          if debug then writeln("Setting end to ", i-1);
	  end = i-1;
	  return 1;
	}
      } else if data[i] == 0 {
        if debug then writeln("Found end of data at ", i);
        end = i-1;
	return 2;
      }
    }

    if debug then writeln("Advancing cursor");
    // we did not find the end of a sequence; set up for the next read
    cursor = last+1;
    return 0;
  }

  proc copyTail() {
    const tailLen = last - end;
    if debug then writeln("Copying tail of ", tailLen, " from ", end+1..#tailLen);
    var newSeq = new Seq(id = id+1,
                         cursor = 0,
                         last = tailLen-1);
    newSeq.data[0..#tailLen] = data[end+1..#tailLen];

    return newSeq;
  }

  proc revcomp() {
    if debug then writeln("Processing sequence from ", start, " to ", end);
    const last = end;
    do {
      start += 1;
    } while data[start] != eol;

    while start <= end {
      ref d1 = data[start],
          d2 = data[end];

      (d1, d2) = (table[d2], table[d1]);

      advance(start, 1);
      advance(end, -1);

      proc advance(ref ind, dir) {
        do {
          ind += dir;
        } while data[ind] == eol;
      }
    }

    seqToWrite.waitFor(id);
    stdoutBin.write(data[..last]);
    stdoutBin.flush();
    seqToWrite.write(id+1);
  }
}


proc main(args: [] string) {
  const stdin = openfd(0),
        input = stdin.reader(iokind.native, locking=false,
                             hints=QIO_HINT_PARALLEL);

  // if the file isn't empty, wait for all tasks to complete before continuing
  if stdin.length() != 0 then sync {
    var curSeq = new Seq(id=1);

    do {
      const eof = curSeq.readChunk(input);
      // TODO: Can we use a while...do here or is that a problem w.r.t. curSeq?
      do {
        const foundSeq = curSeq.processChunk();
        if (foundSeq) {
	  var lastSeq = curSeq;
          curSeq = lastSeq.copyTail();
          begin with (in lastSeq) { lastSeq.revcomp(); }
        }
      } while foundSeq == 1;
    } while !eof;
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
