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
      maxChars = cmpl.size;

var pairCmpl: [0..<join(maxChars, maxChars)] uint(16);

var stdinBin  = openfd(0).reader(iokind.native, locking=false,
                           hints=ioHintSet.fromFlag(QIO_CH_ALWAYS_UNBUFFERED)),
    stdoutBin = openfd(1).writer(iokind.native, locking=false,
                           hints=ioHintSet.fromFlag(QIO_CH_ALWAYS_UNBUFFERED));

proc main(args: [] string) {
  const chars = eol..<maxChars;
  forall (i,j) in {chars, chars} do
    pairCmpl[join(i,j)] = join(cmpl(j), cmpl(i));

  var buffCap = readSize,
      buffDom = {0..<buffCap},
      buff: [buffDom] uint(8),
      readPos = 0;

  do {
    // TODO: Would prefer to use the array form of readBinary(), but it
    // doesn't currently return the number of elements read on EOF...
    var newChars = stdinBin.readBinary(c_ptrTo(buff[readPos]), readSize),
        nextSeqStart: int;

    // TODO: would really just like an array.find() routine rather
    // than writing my own
    while findSeqStart(buff, readPos, newChars, nextSeqStart) {
      revcomp(buff, nextSeqStart);

      newChars -= nextSeqStart - readPos + 1;

      // TODO: how much impact is this forall?
      // TODO: abstract into a mem-move type of method on arrays?
      serial (nextSeqStart < newChars) do
        forall j in 0..newChars do
          buff[j] = buff[j+nextSeqStart];

      readPos = 1;
    }

    readPos += newChars;

    if readPos + readSize > buffCap {
      buffCap *= 2;
      buffDom = {0..<buffCap};
    }
  } while newChars;

  if readPos then revcomp(buff, readPos);
}

proc revcomp(seq, size) {
  param chunkSize = linesPerChunk*cols;

  var headerSize = 1;
  while seq[headerSize-1] != eol {
    headerSize += 1;
  }

  stdoutBin.writeBinary(c_ptrTo(seq[0]), headerSize);

  var charsLeft, charsWritten: atomic int = size - headerSize;

  coforall tid in 0..<here.maxTaskPar {
    var myChunk: [0..<chunkSize] uint(8);

    while true {
      var myStartChar = charsLeft.read();
      while myStartChar > 0 &&
            !charsLeft.compareExchange(myStartChar, myStartChar-chunkSize) { }

      if myStartChar < 0 then break;

      const myChunkSize = min(chunkSize, myStartChar),
            lastLineChars = (myStartChar - 1) % cols,
            lastLineGaps = cols - (lastLineChars + 1);

      var cursor = myStartChar + headerSize - 1,
          chunkLeft = myChunkSize,
          chunkPos = 0;

      if !lastLineGaps {
        revcomp(chunkPos, cursor, chunkLeft, myChunk, seq);
        chunkLeft = 0;
      }

      while chunkLeft >= cols {
        revcomp(chunkPos, cursor, lastLineChars, myChunk, seq);
        chunkPos += lastLineChars;
        cursor -= lastLineChars+1;

        revcomp(chunkPos, cursor, lastLineGaps, myChunk, seq);
        chunkPos += lastLineGaps;
        cursor -= lastLineGaps;

        myChunk[chunkPos] = eol;
        chunkPos += 1;

        chunkLeft -= cols;
      }

      if chunkLeft then
        revcomp(chunkPos, cursor, lastLineChars+1, myChunk, seq);

      charsWritten.waitFor(myStartChar);
      stdoutBin.writeBinary(c_ptrTo(myChunk[0]), myChunkSize);
      charsWritten.write(myStartChar-myChunkSize);
    }
  }
}

proc revcomp(in dstFront, in charAfter, spanLen, myChunk, seq) {
  if spanLen%2 {
    charAfter -= 1;
    myChunk[dstFront] = cmpl[seq[charAfter]];
    dstFront += 1;
  }

  for 2..spanLen by -2 {
    charAfter -= 2;
    const src = c_ptrTo(seq[charAfter]): c_ptr(uint(16)),
          dst = c_ptrTo(myChunk[dstFront]): c_ptr(uint(16));
    dst.deref() = pairCmpl[src.deref()];
    dstFront += 2;
  }
}

proc findSeqStart(buff, low, count, ref ltOff) {
  ltOff = max(int);
  forall i in low..#count with (min reduce ltOff) {
    if buff[i] == '>'.toByte() && i != 0 {
      ltOff = i;
    }
  }
  return ltOff != max(int);
}

inline proc join(i: uint(16), j) {
  return i << 8 | j;
}
