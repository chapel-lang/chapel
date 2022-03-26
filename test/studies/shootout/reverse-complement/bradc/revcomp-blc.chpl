/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Brad Chamberlain
   based on the C gcc #5 version by Mr Ledrug
   and the Chapel #2 version by myself and Ben Harshbarger
*/

use IO;

const stdinBin  = openfd(0).reader(iokind.native, locking=false,
                                   hints = QIO_CH_ALWAYS_UNBUFFERED),
      stdoutBin = openfd(1).writer(iokind.native, locking=false,
                                   hints=QIO_CH_ALWAYS_UNBUFFERED);
param cmpl = b"                                                             " +
             b"    TVGH  CD  M KN   YSAABW R       TVGH  CD  M KN   YSAABW R";
            //     ↑↑↑↑  ↑↑  ↑ ↑↑   ↑↑↑↑↑↑ ↑       ↑↑↑↑  ↑↑  ↑ ↑↑   ↑↑↑↑↑↑ ↑
            //     ABCDEFGHIJKLMNOPQRSTUVWXYZ      abcdefghijklmnopqrstuvwxyz

config var readSize = 16384, n: int;

var nextToPrint: atomic int = 1,
    arrayCopied: atomic bool = false;

proc main() {
  // read in the data using an incrementally growing buffer
  var bufSize = readSize,
      bufDom = {0..<bufSize},
      buf: [bufDom] uint(8),
      seqNum, seqStart, totProcessed, totRead = 0,
      end = -1;

  do {
    const start = end + 1,
          more = stdinBin.read(buf[start..#readSize]);
    if !more then
      readSize = stdinBin.offset() - totRead + 1;
    else
      totRead += readSize;

    do {
      end += 1;
      // TODO: Problem: We re-read the old leftover '>' from having
      // shifted the buffer
      if end != 0 && buf[end] == '>'.toByte() {
        seqNum += 1;
        totProcessed += end-seqStart;
        // really want:
        //   begin with (var seq = buf[seqStart..<end])
        //     revcomp(stdoutBin, seq);
        begin revcomp(seqNum, buf[seqStart..<end]);
        arrayCopied.waitFor(true);
        arrayCopied.write(false);
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
  seqNum += 1;
  const len = stdinBin.offset()-totProcessed;
  revcomp(seqNum, buf[seqStart..<end]);
}

proc revcomp(seqID, in buf) {
  var seq = buf;
  arrayCopied.waitFor(false);
  arrayCopied.write(true);
  param eol  = '\n'.toByte();      // end-of-line, as an integer
  var lo = seq.indices.low,
      hi = seq.indices.high;

  if lo > hi then return;
  
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
//    stdoutBin.writeln("buf[lo] = ", buf[lo]);
//    stdoutBin.writeln("buf[hi] = ", buf[hi]);
    (buf[lo], buf[hi]) = (cmpl(buf[hi]), cmpl(buf[lo]));
//    stdoutBin.writeln("buf[lo] = ", buf[lo]);
//    stdoutBin.writeln("buf[hi] = ", buf[hi]);
  }
  nextToPrint.waitFor(seqID);
  stdoutBin.write(buf);
  nextToPrint.add(1);
}
