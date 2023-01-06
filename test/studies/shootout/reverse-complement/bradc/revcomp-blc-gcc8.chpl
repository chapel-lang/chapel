/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Brad Chamberlain
   based on the C gcc #8 version by Jeremy Zerfas
*/

use CTypes, IO;

config param readSize = 65536,
             linesPerChunk = 8192,
             n = 0;  // TODO: How can this work?  Why does --n=0 work?

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
  forall i in chars do
    foreach j in chars do
      pairCmpl[join(i,j)] = join(cmpl(j), cmpl(i));

  var buffCap = readSize,
      buffDom = {0..<buffCap},
      buff: [buffDom] uint(8),
      endOfSeq = 0;

  do {
    var newChars = stdinBin.readBinary(c_ptrTo(buff[endOfSeq]), readSize),
        nextSeq: int;

    while findSeqStart(buff, endOfSeq, newChars, nextSeq) {
      revcomp(buff, nextSeq);

      newChars -= nextSeq - endOfSeq + 1;

      // TODO: how much impact?
      // TODO: abstract into a mem-move type of method on arrays?
      serial (nextSeq < newChars) do
        forall j in 0..newChars do
          buff[j] = buff[j+nextSeq];

      endOfSeq = 1;
    }

    endOfSeq += newChars;

    if endOfSeq + readSize > buffCap {
      buffCap *= 2;
      buffDom = {0..<buffCap};
    }
  } while newChars;

  if endOfSeq {
    revcomp(buff, endOfSeq);
  }
}

proc revcomp(seq, size) {
  param chunkSize = linesPerChunk*cols;

  var headerSize = 0;
  while seq[headerSize] != eol {
    headerSize += 1;
  }
  stdoutBin.write(seq[0..headerSize]);

  var charsLeft, charsWritten: atomic int = size-(headerSize+1);

  coforall tid in 0..<here.maxTaskPar {
    var myChunk: [0..<chunkSize] uint(8);

    do {
      var myStartChar = charsLeft.read(),
          myChunkSize = min(chunkSize, myStartChar);

      do {
        if myStartChar <= 0 then
          break;
      } while !charsLeft.compareExchange(myStartChar, myStartChar-chunkSize);


      if myStartChar > 0 {
        const lastLineChars = (myStartChar-1)%cols,
              lastLineGaps = cols-1-lastLineChars;

        var lastProc = myStartChar + headerSize,
            chunkLeft = myChunkSize,
            chunkPos = 0;

        if !lastLineGaps {
          revcompHelp(chunkPos, lastProc, chunkLeft, myChunk, seq);
          chunkLeft = 0;
        }

        // TODO: Could this be a strided while loop?
        while chunkLeft >= cols {
          revcompHelp(chunkPos, lastProc, lastLineChars, myChunk, seq);
          chunkPos += lastLineChars;
          lastProc -= lastLineChars+1;
          
          revcompHelp(chunkPos, lastProc, lastLineGaps, myChunk, seq);
          chunkPos += lastLineGaps;
          lastProc -= lastLineGaps;
          
          myChunk[chunkPos] = eol;
          chunkPos += 1;
          
          chunkLeft -= cols;
        }
        
        if chunkLeft {
          revcompHelp(chunkPos, lastProc, lastLineChars+1, myChunk, seq);
        }

        charsWritten.waitFor(myStartChar);
        stdoutBin.write(myChunk[0..<myChunkSize]);
        charsWritten.write(myStartChar-myChunkSize);
      }
    } while myStartChar > 0;
  }
}

proc revcompHelp(in dstFront, in charAfter, in spanLen, myChunk, seq) {
  if spanLen%2 {
    charAfter -= 1;
    myChunk[dstFront] = cmpl[seq[charAfter]];
    dstFront += 1;
  }

  while spanLen >= 2 {
    charAfter -= 2;
    const pair = ((c_ptrTo(seq[charAfter])):c_ptr(uint(16))).deref();
    const dest = c_ptrTo(myChunk[dstFront]):c_ptr(uint(16));
    dest.deref() = pairCmpl[pair];
    spanLen -= 2;
    dstFront += 2;
  }
}


config const useMemChr = false;

// TODO: any clever way to avoid the inds.low conditional?
proc findSeqStart(buff, in low, in count, ref ltOff, locUseMemChr = useMemChr) {
  // TODO: this seems silly... must be some way to avoid?
  // TODO: If we can, could make 'in' arguments not be anymore
  if low == 0 {
    low += 1;
    count -= 1;
    if count < 0 then
      return false;
  }

  if locUseMemChr {
    extern proc memchr(s, c, n): c_void_ptr;
    var ptr = memchr(c_ptrTo(buff[low]), '>'.toByte(), count);
    if ptr == c_nil then
      return false;
    else {
      ltOff = ptr: c_ptr(uint(8)) - c_ptrTo(buff[low]) + 1;
      return true;
    }
  } else {
    var (val, loc) = maxloc reduce zip([i in low..#count]
                                       buff[i] == '>'.toByte(),
                                       low..#count);
    if val {
      ltOff = loc;
      return true;
    } else {
      return false;
    }
  }
}


inline proc join(i:uint(16), j) {
  return i << 8 | j;
}

