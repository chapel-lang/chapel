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

var seqToPrint: atomic int = 0;

proc main() {
  // read in the data using an incrementally growing buffer
  var bufSize = readSize,
      bufDom = {0..<bufSize},
      buf: [bufDom] uint(8),
      seqStart, totRead, nextSeqID = 0,
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
        // TODO: This latch is heavy-handed... we really want:
        //   begin with (var seq = buf[seqStart..<end])
        //     revcomp(stdoutBin, seq);
        // or maybe even just:
        //   begin revcomp(stoutBin, seq); ???
        //
        // Unfortunate: Have to make a double copy of seq (to avoid
        // sharing a domain with buf and possibly getting resized
        // within our begin when it does.  TPVs would help with
        // this.
        var seq = buf[seqStart..<end];
        begin with (in nextSeqID, in seq, in seqStart, in end)
          revcomp(nextSeqID, seq, seqStart, end);
//        use Time;
//        sleep(2);
        nextSeqID += 1;
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
  revcomp(nextSeqID, buf[seqStart..<end], seqStart, end);
}

proc revcomp(seqID, seq, in lo, in hi) {
  param eol  = '\n'.toByte();      // end-of-line, as an integer

  if lo >= hi then return;

  // skip past header line
  while seq[lo] != eol do
    lo += 1;

  while lo < hi {
    do {
      lo += 1;
    } while seq[lo] == eol;
    do {
      hi -= 1;
    } while seq[hi] == eol;
    if lo < hi then
      (seq[lo], seq[hi]) = (cmpl(seq[hi]), cmpl(seq[lo]));
  }
  seqToPrint.waitFor(seqID);
  stdoutBin.write(seq);
  seqToPrint.add(1);
}
