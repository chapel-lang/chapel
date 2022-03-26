/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Brad Chamberlain
   based on the C gcc #5 version by Mr Ledrug
   and the Chapel #2 version by myself and Ben Harshbarger
*/

use IO;

param eol = '\n'.toByte();      // end-of-line, as an integer

param cmpl = b"          \n                                                 "
            //      ABCDEFGHIJKLMNOPQRSTUVWXYZ      abcdefghijklmnopqrstuvwxyz
           + b"     TVGH  CD  M KN   YSAABW R       TVGH  CD  M KN   YSAABW R";

config const readSize = 16384; // 10000; // 10;  // 16384;  // TODO: replace with 16384

proc main(args: []) {
  const stdinBin = openfd(0).reader(iokind.native, locking=false,
                                 hints = QIO_CH_ALWAYS_UNBUFFERED),
        stdoutBin = openfd(1).writer(iokind.native, locking=false,
                                  hints=QIO_CH_ALWAYS_UNBUFFERED);

  // read in the data using an incrementally growing buffer
  var bufSize = readSize,
      bufDom = {0..<bufSize},
      buf: [bufDom] uint(8),
      seqNum, seqStart, totProcessed = 0,
      end = -1;

  do {
    const start = end+1,
          more = stdinBin.read(buf[start..#readSize]);
//    stdoutBin.writeln("Reading into buffer from ", start:string, "..", (start+readSize-1):string);
//    stdoutBin.writeln("Read: ", buf[start..#readSize]);
//    stdoutBin.writeln("Looking for '>'");
    do {
      end += 1;
      if buf[end] == '>'.toByte() {
//        stdoutBin.writeln("found one!");
        seqNum += 1;
        //        stdoutBin.stdoutBin.write("Sequence ", seqNum:string, " is:\n", buf[seqStart..<end]);
        totProcessed += end-seqStart;
        revcompCopy(stdoutBin, buf[seqStart..<end]);
        seqStart = end;
//        stdoutBin.writeln("looking for another");
      }
    } while end < start+readSize-1;
//    stdoutBin.writeln("didn't find one");

    if more {
//      stdoutBin.writeln("The file holds more");
      // if we processed one or more sequences, shift leftovers down
      if seqStart != 0 {
//        stdoutBin.writeln("Shift what we've read");
        for (s,d) in zip(seqStart..<start+readSize, 0..) do
          buf[d] = buf[s];
        end -= seqStart;
        seqStart = 0;
//        stdoutBin.writeln("Leftovers: ", buf[0..<end]);
      }

      // resize if necessary
      if end + readSize > bufSize {
//        stdoutBin.writeln("Resize the buffer");
        bufSize *= 2;
        bufDom = {0..<bufSize};
      }
    } else {
//      stdoutBin.writeln("Fell off the end");
//      stdoutBin.writeln("Fell off the end");
      seqNum += 1;
      const len = stdinBin.offset()-totProcessed;
      ////      stdoutBin.write("Sequence ", seqNum:string, " is:\n", buf[seqStart..<end]);
      /*
      stdoutBin.writeln("Final sequence starts at: ", seqStart:string);
      stdoutBin.writeln("Final sequence ends at: ", len:string);
      stdoutBin.writeln("Offset in stdin was: ", stdinBin.offset():string);
      stdoutBin.writeln("Tot read was: ", totProcessed:string);
*/
      revcomp(stdoutBin, buf[seqStart..#len]);
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
  stdoutBin.write(buf);
}
