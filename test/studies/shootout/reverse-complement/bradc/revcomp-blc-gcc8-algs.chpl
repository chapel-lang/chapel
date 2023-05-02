/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Brad Chamberlain
   based on the C gcc #8 version by Jeremy Zerfas
*/

use IO, CTypes;

config param readSize = 65536,
             linesPerChunk = 8192;

enum alg {Spin, WaitFor, SerForall, Forall, Foreach, For, MemMove, MaxLoc,
          OptForall, MemChr, Find};
use alg;

// shift:
// * SerForall
// ? Forall
// ? Foreach
// * For
// * MemMove

// wait:
// * Spin
// * WaitFor

// search:
// * Forall
// ? OptForall
// * Foreach
// * For
// * MemChr

config param shiftAlg = SerForall, // can also be Forall, Foreach, For, MemMove
             waitAlg = WaitFor,    // can also be Spin
             searchAlg = MaxLoc;   // can also be Forall, OptForall, Foreach,
                                   //             For, MemChr
                                   // TODO: could also do:
                                   // - coforall+MemChr
                                   // - coforall+For[each]

config const verbose = false;


if verbose then
  stderr.writeln("Algorihm set: ", (shiftAlg, waitAlg, searchAlg));

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
var stdinBin  = (new file(0)).reader(iokind.native, locking=false,
                                     hints=ioHintSet.fromFlag(QIO_CH_ALWAYS_UNBUFFERED)),
  stdoutBin = (new file(1)).writer(iokind.native, locking=false,
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
    var endOfRead = readPos + newChars;
    while findSeqStart(buff, max(readPos,1)..<endOfRead, nextSeqStart) {
      // process this one
      revcomp(buff, nextSeqStart);

      // then reset to check for the next sequence
      readPos = 0;
      endOfRead -= nextSeqStart;

      // shift the remaining characters down (only in parallel if no overlap)
      if shiftAlg == SerForall {
        serial (nextSeqStart < endOfRead) do
          forall j in 0..<endOfRead do
            buff[j] = buff[j + nextSeqStart];
      } else if shiftAlg == Forall {
        // warning: not safe in general, but known to be for perf input
        // (but not for all practice inputs, I think?)
        forall j in 0..<endOfRead do
          buff[j] = buff[j + nextSeqStart];
     } else if shiftAlg == Foreach {
        // warning: not safe in general, but known to be for perf input
        // (but not for all practice inputs, I think?)
        foreach j in 0..<endOfRead do
          buff[j] = buff[j + nextSeqStart];
     } else if shiftAlg == For {
        for j in 0..<endOfRead do
          buff[j] = buff[j + nextSeqStart];
     } else if shiftAlg == MemMove {
       extern proc memmove(dst, src, len);
       memmove(c_ptrTo(buff[0]), c_ptrTo(buff[nextSeqStart]), endOfRead);
     } else {
       compilerError("Unexpected shift algorithm: " + shiftAlg:string);
     }
       
        
    }

    // update where we'll read from next
    readPos = endOfRead;

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
  param chunkSize = linesPerChunk * cols; // the size of the chunks to deal out

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

    // grab  chunk of work
    var myStartChar = charsLeft.fetchSub(chunkSize);
    while myStartChar >= 0 {
      const myChunkSize = min(chunkSize, myStartChar + 1),
            lastLineChars = myStartChar % cols,        // # of chars and
            lastLineGaps = cols - (lastLineChars + 1); // spaces on last line

      var cursor = myStartChar + headerSize,
          chunkLeft = myChunkSize,
          chunkPos = 0;

      // reverse the chunk
      if lastLineGaps == 0 {
        // if last line is full, it's easy to reverse-complement
        revcomp(chunkPos, cursor, chunkLeft, myBuff, seq);
      } else {
        // otherwise, it's trickier
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

      // take turns writing out our chunks
      if waitAlg == WaitFor {
        charsWritten.waitFor(myStartChar);
      } else if waitAlg == Spin {
        while charsWritten.read() != myStartChar {}
      } else {
        compilerError("Unexpected wait algorithm: " + waitAlg:string);
      }
      stdoutBin.writeBinary(c_ptrTo(myBuff[0]), myChunkSize);
      charsWritten.write(myStartChar - myChunkSize);

      // grab the next chunk of work
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
    const src = c_ptrTo(seq[charAfter]):c_void_ptr:c_ptr(uint(16)),
          dst = c_ptrTo(buff[dstFront]):c_void_ptr:c_ptr(uint(16));
    dst.deref() = pairCmpl[src.deref()];
    dstFront += 2;
  }
}

// check whether there's a sequence start ('>') in 'buff[low..#count]'
proc findSeqStart(buff, inds, ref ltLoc) {
  if searchAlg == MaxLoc {
    var found: bool;
    (found, ltLoc) = maxloc reduce zip([i in inds] buff[i] == '>'.toByte(),
                                       inds);
    return found;
  } else if searchAlg == Forall {
    ltLoc = max(int);
    forall i in inds with (min reduce ltLoc) {
      if buff[i] == '>'.toByte() {
        ltLoc = min(ltLoc, i);
      }
    }
    return ltLoc != max(int);
  } else if searchAlg == OptForall {
    // warning: This will only work if the sequences are long enough that
    // no task will have multiple '>' characters in its chunk (true for
    // all inputs on the site)
    ltLoc = max(int);
    forall i in inds with (min reduce ltLoc) {
      if buff[i] == '>'.toByte() {
        ltLoc = i;
      }
    }
    return ltLoc != max(int);
    
  } else if searchAlg == Foreach {
    // this requires reduce intents. The solution here is potentially race-y.
    // Making it use a `for` instead of `foreach` results in what looks like an
    // infinite loop. This mode is commented out in the compopts
    foreach i in inds {
      if buff[i] == '>'.toByte() {
        ltLoc = i;
      }
    }
    return ltLoc != max(int);
  } else if searchAlg == For {
    for i in inds {
      if buff[i] == '>'.toByte() {
        ltLoc = i;
        return true;
      }
    }
    return false;
  } else if searchAlg == MemChr {
    extern proc memchr(s, c, n): c_void_ptr;

    const lowptr = c_ptrTo(buff[inds.low]),
          zeroptr = c_ptrTo(buff[0]),
          ltptr = memchr(lowptr, '>'.toByte(), inds.size): c_ptr(uint(8));

    ltLoc = ltptr - zeroptr;
    return ltptr != c_nil;
  } else if searchAlg == Find {
    ltLoc = buff[inds].find('>'.toByte());
    return ltLoc != inds.low-1;
  } else {
    compilerError("Unexpected search algorithm" + searchAlg: string);
  }
}

// combine two nucleotide characters to create a 16-bit integer
inline proc join(i: uint(16), j) {
  return i << 8 | j;
}
