/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Brad Chamberlain
   based on the C gcc #6 version by Jeremy Zerfas
*/

use IO;

const stdinBin  = openfd(0).reader(iokind.native, locking=false,
                                   hints = QIO_CH_ALWAYS_UNBUFFERED),
      stdoutBin = openfd(1).writer(iokind.native, locking=false,
                                   hints=QIO_CH_ALWAYS_UNBUFFERED);

// A 'bytes' value that stores the complement of a base at its given index
param cmpl = b"                                                             " +
             b"    TVGH  CD  M KN   YSAABW R       TVGH  CD  M KN   YSAABW R";
            //     ↑↑↑↑  ↑↑  ↑ ↑↑   ↑↑↑↑↑↑ ↑       ↑↑↑↑  ↑↑  ↑ ↑↑   ↑↑↑↑↑↑ ↑
            //     ABCDEFGHIJKLMNOPQRSTUVWXYZ      abcdefghijklmnopqrstuvwxyz

config var readSize = 16384, // how much to read at a time
           n = 0;            // a dummy variable to match the CLBG framework

proc main() {
  // read in the data using an incrementally growing buffer
  var bufSize = readSize,
      bufDom = {0..<bufSize},
      buf: [bufDom] uint(8),
      seqStart, totRead = 0,
      end = -1;  // TODO: Can this be made into a 0...

  do {
    const start = end + 1,  // TODO: ...to drop this + 1...
          more = stdinBin.read(buf[start..#readSize]);
    if !more then
      readSize = stdinBin.offset() - totRead + 1;
    else
      totRead += readSize;

    do {
      end += 1; // TODO: If we move this to the bottom of the loop?
      if end != 0 && buf[end] == '>'.toByte() {  // TODO: and drop this !=?
        revcomp(buf, seqStart, end);
        seqStart = end;
      }
    } while end < start+readSize-1;

    if more {
      // if we processed one or more sequences, shift leftovers down
      if seqStart != 0 {
        for (s,d) in zip(seqStart..<start+readSize, 0..) do
          buf[d] = buf[s];
        end -= seqStart;
        seqStart = 0;
      }

      // resize if necessary
      if end + readSize >= bufSize {
        bufSize *= 2;
        bufDom = {0..<bufSize};
      }
    }
  } while more;
  revcomp(buf, seqStart, end);
}

proc revcomp(buf, in lo, in hi) {
  param eol  = '\n'.toByte();      // end-of-line, as an integer

  if lo >= hi then return;

  ref seq = buf[lo..<hi];

  // skip past header line
  while buf[lo] != eol do
    lo += 1;

  while lo < hi {
    do {
      lo += 1;
    } while buf[lo] == eol;
    do {
      hi -= 1;
    } while buf[hi] == eol;
    if lo < hi then
      (buf[lo], buf[hi]) = (cmpl(buf[hi]), cmpl(buf[lo]));
  }
  stdoutBin.write(seq);
}
