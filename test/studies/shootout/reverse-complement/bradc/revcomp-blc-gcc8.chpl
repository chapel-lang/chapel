/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Brad Chamberlain
   based on the C gcc #8 version by Jeremy Zerfas
*/

use IO, CTypes;

config param readSize = 65536,
             linesPerChunk = 8192;

param eol = '\n'.toByte(),  // end-of-line, as an integer
      cols = 61,            // # of characters per full row (including '\n')

      // A 'bytes' value that stores the complement of each base at its index
      cmpl = b"          \n                                                  "
           + b"    TVGH  CD  M KN   YSAABW R       TVGH  CD  M KN   YSAABW R",
             //    ↑↑↑↑  ↑↑  ↑ ↑↑   ↑↑↑↑↑↑ ↑       ↑↑↑↑  ↑↑  ↑ ↑↑   ↑↑↑↑↑↑ ↑
             //    ABCDEFGHIJKLMNOPQRSTUVWXYZ      abcdefghijklmnopqrstuvwxyz

      maxChars = cmpl.size; // upper bound on number of nucleotides used

// map from pairs of nucleotide characters to their reversed complements
var pairCmpl: [0..<join(maxChars, maxChars)] uint(16);

// channels for doing efficient console I/O
var stdinBin  = openfd(0).reader(iokind.native, locking=false,
                           hints=ioHintSet.fromFlag(QIO_CH_ALWAYS_UNBUFFERED)),
    stdoutBin = openfd(1).writer(iokind.native, locking=false,
                           hints=ioHintSet.fromFlag(QIO_CH_ALWAYS_UNBUFFERED));

proc main(args: [] string) {
  // set up the 'pairCmpl' map
  const chars = eol..<maxChars;
  forall (i,j) in {chars, chars} do
    pairCmpl[join(i,j)] = join(cmpl(j), cmpl(i));

  // variables for reading into a dynamically growing buffer
  var buffCap = readSize,       // capacity of our input buffer
      buffDom = {0..<buffCap},  // index set for the input buffer
      buff: [buffDom] uint(8),  // the buffer itself
      readPos = 0;              // the current read position

  do {
    // read 'readSize' new characters
    var newChars = stdinBin.readBinary(c_ptrTo(buff[readPos]), readSize),
        nextSeqStart: int;

    // if the new characters contain the start of the next sequence,
    var stop = readPos+newChars;
    while findSeqStart(buff, max(readPos,1)..<stop, nextSeqStart) {
      // process this one
      revcomp(buff, nextSeqStart);

      // then shift the next sequence to the start of the buffer
      const extras = stop - nextSeqStart;
      stop = extras;

      serial (nextSeqStart < extras) do
        forall j in 0..<extras do
          buff[j] = buff[j+nextSeqStart];

      // and reset to see whether there's another sequence ahead
      readPos = 0;
    }

    // update the position to read to next
    readPos = stop;

    // if we're about to run out of space, grow the buffer by 2x
    if readPos + readSize > buffCap {
      buffCap *= 2;
      buffDom = {0..<buffCap};
    }
  } while newChars;

  // if anything remains, process it
  if readPos then revcomp(buff, readPos);
}

proc revcomp(seq, size) {
  param chunkSize = linesPerChunk*cols;  // the size of the chunks to deal out

  // compute how big the header is
  var headerSize = 1;
  while seq[headerSize - 1] != eol {
    headerSize += 1;
  }

  // write out the header
  stdoutBin.writeBinary(c_ptrTo(seq[0]), headerSize);

  // set up the atomic variables we'll use to coordinate between tasks
  var charsLeft, charsWritten: atomic int = size - (headerSize + 1);

  // create a task per core
  coforall tid in 0..<here.maxTaskPar {
    var myBuff: [0..<chunkSize] uint(8);  // the task's buffer

    var myStartChar = charsLeft.fetchSub(chunkSize);
    while myStartChar >= 0 {
      const myChunkSize = min(chunkSize, myStartChar + 1),
            lastLineChars = myStartChar % cols,
            lastLineGaps = cols - (lastLineChars + 1);

      var cursor = myStartChar + headerSize,
          chunkLeft = myChunkSize,
          chunkPos = 0;

      if lastLineGaps == 0 {
        revcomp(chunkPos, cursor, chunkLeft, myBuff, seq);
      } else {
        while chunkLeft >= cols {
          revcomp(chunkPos, cursor, lastLineChars, myBuff, seq);
          chunkPos += lastLineChars;
          cursor -= lastLineChars + 1;

          revcomp(chunkPos, cursor, lastLineGaps, myBuff, seq);
          chunkPos += lastLineGaps;
          cursor -= lastLineGaps;

          myBuff[chunkPos] = eol;
          chunkPos += 1;

          chunkLeft -= cols;
        }

        if chunkLeft then
          revcomp(chunkPos, cursor, lastLineChars + 1, myBuff, seq);
      }

      while charsWritten.read() != myStartChar {}
      stdoutBin.writeBinary(c_ptrTo(myBuff[0]), myChunkSize);
      charsWritten.write(myStartChar-myChunkSize);

      myStartChar = charsLeft.fetchSub(chunkSize);
    }
  }
}

proc revcomp(in dstFront, in charAfter, spanLen, buff, seq) {
  if spanLen%2 {
    charAfter -= 1;
    buff[dstFront] = cmpl[seq[charAfter]];
    dstFront += 1;
  }

  for 2..spanLen by -2 {
    charAfter -= 2;
    const src = c_ptrTo(seq[charAfter]): c_ptr(uint(16)),
          dst = c_ptrTo(buff[dstFront]): c_ptr(uint(16));
    dst.deref() = pairCmpl[src.deref()];
    dstFront += 2;
  }
}

// see whether there's a sequence start ('>') in 'buff[low..#count]'
proc findSeqStart(buff, inds, ref ltOff) {
  ltOff = max(int);
  forall i in inds with (min reduce ltOff) {
    if buff[i] == '>'.toByte() {
      ltOff = min(ltOff, i);
    }
  }
  return ltOff != max(int);
}

// combine two nucleotide characters to create a 16-bit integer
inline proc join(i: uint(16), j) {
  return i << 8 | j;
}
