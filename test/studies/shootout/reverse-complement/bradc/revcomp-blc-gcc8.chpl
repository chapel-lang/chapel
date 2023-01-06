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
      maxNucls = cmpl.size;

var pairCmpl: [0..<join(maxNucls, maxNucls)] uint(16);

var stdinBin  = openfd(0).reader(iokind.native, locking=false,
                           hints=ioHintSet.fromFlag(QIO_CH_ALWAYS_UNBUFFERED)),
    stdoutBin = openfd(1).writer(iokind.native, locking=false,
                           hints=ioHintSet.fromFlag(QIO_CH_ALWAYS_UNBUFFERED));

proc main(args: [] string) {
  const nucls = eol..<maxNucls;
  forall i in nucls do
    foreach j in nucls do
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
  var headerSize = 0;
  while seq[headerSize] != eol {
    headerSize += 1;
  }
  stdoutBin.write(seq[0..headerSize]);

  var charsLeft, writtenChars: atomic int = size-(headerSize+1);

  coforall tid in 0..<here.maxTaskPar {
    var chunkToWrite: [0..<linesPerChunk*cols] uint(8);

    do {
      var myCharsLeft = charsLeft.read(),
          chunkSize = linesPerChunk * cols;

      do {
        if myCharsLeft <= 0 then
          break;
      } while !charsLeft.compareExchange(myCharsLeft, myCharsLeft-chunkSize);


      if myCharsLeft > 0 {
        const fullLineFrontSpanLength = (myCharsLeft-1)%cols,
              fullLineRearSpanLength = cols-1-fullLineFrontSpanLength;

        if chunkSize > myCharsLeft {
          chunkSize = myCharsLeft;
        }

        var lastProc = myCharsLeft + headerSize,
            chunkLeft = chunkSize,
            chunkPos = 0;

        if !fullLineRearSpanLength {
          revcompHelp(chunkPos, lastProc, chunkLeft, chunkToWrite, seq);
          chunkLeft = 0;
        }

        // TODO: Could this be a strided while loop?
        while chunkLeft >= cols {
          revcompHelp(chunkPos, lastProc, fullLineFrontSpanLength, chunkToWrite, seq);
          chunkPos += fullLineFrontSpanLength;
          lastProc -= fullLineFrontSpanLength+1;
          
          revcompHelp(chunkPos, lastProc, fullLineRearSpanLength, chunkToWrite, seq);
          chunkPos += fullLineRearSpanLength;
          lastProc -= fullLineRearSpanLength;
          
          chunkToWrite[chunkPos] = eol;
          chunkPos += 1;
          
          chunkLeft -= cols;
        }
        
        if chunkLeft {
          revcompHelp(chunkPos, lastProc, fullLineFrontSpanLength+1, chunkToWrite, seq);
        }

        writtenChars.waitFor(myCharsLeft);
        stdoutBin.write(chunkToWrite[0..<chunkSize]);
        writtenChars.write(myCharsLeft-chunkSize);
      }
    } while myCharsLeft > 0;
  }
}

proc revcompHelp(in dstFront, in charAfter, in spanLen, chunkToWrite, seq) {
  if spanLen%2 {
    charAfter -= 1;
    chunkToWrite[dstFront] = cmpl[seq[charAfter]];
    dstFront += 1;
  }

  while spanLen >= 2 {
    charAfter -= 2;
    const pair = ((c_ptrTo(seq[charAfter])):c_ptr(uint(16))).deref();
    const dest = c_ptrTo(chunkToWrite[dstFront]):c_ptr(uint(16));
    dest.deref() = pairCmpl[pair];
    spanLen -= 2;
    dstFront += 2;
  }
}


config const useMemChr = false;

// TODO: any clever way to avoid the inds.low conditional?
proc findSeqStart(chunk, in low, in count, ref ltOff, locUseMemChr = useMemChr) {
  // this seems silly... must be some way to avoid?
  if low == 0 {
    low += 1;
    count -= 1;
    if count < 0 then
      return false;
  }

  if locUseMemChr {
    extern proc memchr(s, c, n): c_void_ptr;
    var ptr = memchr(c_ptrTo(chunk[low]), '>'.toByte(), count);
    if ptr == c_nil then
      return false;
    else {
      ltOff = ptr: c_ptr(uint(8)) - c_ptrTo(chunk[low]) + 1;
      return true;
    }
  } else {
    var (val, loc) = maxloc reduce zip([i in low..#count]
                                       chunk[i] == '>'.toByte(),
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

