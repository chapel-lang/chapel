/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Brad Chamberlain
   based on the C gcc #5 version by Mr Ledrug
   and the Chapel #2 version by myself and Ben Harshbarger
*/

use IO;

config const debug = false, rc = !debug;

param eol = '\n'.toByte();      // end-of-line, as an integer

param cmpl = b"          \n                                                 "
            //      ABCDEFGHIJKLMNOPQRSTUVWXYZ      abcdefghijklmnopqrstuvwxyz
           + b"     TVGH  CD  M KN   YSAABW R       TVGH  CD  M KN   YSAABW R";

config var readSize = 16384, n: int; // 10;  // 16384;  // TODO: replace with 16384

proc main() {
  const stdinBin = openfd(0).reader(iokind.native, locking=false,
                                 hints = QIO_CH_ALWAYS_UNBUFFERED),
        stdoutBin = openfd(1).writer(iokind.native, locking=false,
                                  hints=QIO_CH_ALWAYS_UNBUFFERED);

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
      readSize = stdinBin.offset() - totRead;
    else
      totRead += readSize;
    if debug {
      stdoutBin.writeln("*** Reading into buffer from ", start:string, "..", (start+readSize-1):string);
      if readSize > 0 then
        stdoutBin.writeln("*** Read: ", buf[start..#readSize]);
      stdoutBin.writeln("*** Looking for '>'");
    }
    do {
      end += 1;
      // TODO: Problem: We re-read the old leftover '>' from having
      // shifted the buffer
      if end != 0 && buf[end] == '>'.toByte() {
        if debug then
          stdoutBin.writeln("*** found one!");
        seqNum += 1;
        if debug {
          stdoutBin.write("*** Sequence ", seqNum:string, " is:\n", buf[seqStart..<end]);
          stdoutBin.writeln("*** Incrementing totProcessed by ", (end-seqStart):string);
        }
        totProcessed += end-seqStart;
        if rc then
          revcompCopy(stdoutBin, buf[seqStart..<end]);
        seqStart = end;
        if debug then
          stdoutBin.writeln("*** looking for another");
      }
    } while end < start+readSize-1;
    if debug then
      stdoutBin.writeln("*** didn't find one");

    if more {
      if debug then
        stdoutBin.writeln("*** The file holds more");
      // if we processed one or more sequences, shift leftovers down
      if seqStart != 0 {
        if debug then
          stdoutBin.writeln("*** Shift what we've read");
        for (s,d) in zip(seqStart..<start+readSize, 0..) do
          buf[d] = buf[s];
        end -= seqStart;
        seqStart = 0;
        if debug then
          stdoutBin.writeln("*** Leftovers:\n", buf[0..<end]);
      }

      // resize if necessary
      if end + readSize >= bufSize {
        if debug then
          stdoutBin.writeln("*** Resize the buffer");
        bufSize *= 2;
        bufDom = {0..<bufSize};
      }
    } else {
      if debug then
        stdoutBin.writeln("*** Fell off the end");
      end += 1;
      seqNum += 1;
      const len = stdinBin.offset()-totProcessed;
      if debug {
        stdoutBin.write("*** Sequence ", seqNum:string, " is:\n", buf[seqStart..<end]);
        stdoutBin.writeln("*** Final sequence starts at: ", seqStart:string);
        stdoutBin.writeln("*** Final sequence ends at: ", (end-1):string);
        stdoutBin.writeln("*** Offset in stdin was: ", stdinBin.offset():string);
        stdoutBin.writeln("*** Tot read was: ", totProcessed:string);
      }
      /*
> Final sequence starts at: 5131
> Final sequence ends at: -17
> Offset in stdin was: 10245
> Tot read was: 10262
*/
      if rc then
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
