/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Brad Chamberlain
   based on the C gcc #5 version by Mr Ledrug
   and the Chapel #2 version by myself and Ben Harshbarger
*/

use IO;

param eol = "\n".toByte();      // end-of-line, as an integer

param cmpl = b"          \n                                                 "
            //      ABCDEFGHIJKLMNOPQRSTUVWXYZ      abcdefghijklmnopqrstuvwxyz
           + b"     TVGH  CD  M KN   YSAABW R       TVGH  CD  M KN   YSAABW R";

proc main(args: []) {
  const stdinBin = openfd(0).reader(iokind.native, locking=false,
                                 hints = QIO_CH_ALWAYS_UNBUFFERED),
        stdoutBin = openfd(1).writer(iokind.native, locking=false,
                                  hints=QIO_CH_ALWAYS_UNBUFFERED);

  // read in the data using an incrementally growing buffer
  param readSize = 16384;
  var bufSize = readSize,
      bufDom = {0..<bufSize},
      buf: [bufDom] uint(8),
      seqNum, seqStart, end, totRead = 0;

  do {
    const start = end,
          more = stdinBin.read(buf[start..#readSize]);
//    stdoutBin.writeln("Looking for '>'");
    do {
      end += 1;
      if buf[end] == '>'.toByte() {
        seqNum += 1;
//        stdoutBin.write("Sequence ", seqNum:string, " is:\n", buf[seqStart..<end]);
        revcompCopy(stdoutBin, buf[seqStart..<end]);
        seqStart = end;
      }
    } while end < start+readSize-1;

    if more {
      totRead += readSize;
      // if we processed one or more sequences, shift leftovers down
      if seqStart != 0 then
        for (d,s) in zip(seqStart..<end, 0..) do
          buf[d] = buf[s];

      // resize if necessary
      if end + readSize > bufSize {
        bufSize *= 2;
        bufDom = {0..<bufSize};
      }
    } else {
//      stdoutBin.writeln("Fell off the end");
      seqNum += 1;
      end = stdinBin.offset()-totRead;
//      stdoutBin.write("Sequence ", seqNum:string, " is:\n", buf[seqStart..<end]);
      revcomp(stdoutBin, buf[seqStart..<end]);
    }
  } while more;
//  end = stdinBin.offset()-1;
//  revcomp(stdoutBin, buf[0..end]);
}

proc revcompCopy(stdoutBin, in buf) {
  revcomp(stdoutBin, buf);
}

proc revcomp(stdoutBin, buf) {
  param cols = 61;  // the number of characters per full row (including '\n')
  var lo = buf.indices.low,
      hi = buf.indices.high;

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
  stdoutBin.write(buf);
}
