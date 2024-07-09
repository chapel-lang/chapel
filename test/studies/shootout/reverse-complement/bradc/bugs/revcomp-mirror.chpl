/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Brad Chamberlain
   based on the Chapel #3 version with some inspiration taken from the
   C gcc #6 version by Jeremy Zerfas
*/

use IO;

param eol = '\n'.toByte();  // end-of-line, as an integer

proc main(args: [] string) {
  var stdin = (new file(0)).reader(locking=false),

      stdout = (new file(1)).writer(locking=false),

      bufLen = 8 * 1024,
      bufDom = {0..<bufLen},
      buf: [bufDom] uint(8),
      end = 0;

  // read in the data using an incrementally growing buffer
  // Note that this approach ends up with a buffer twice as big as necessary
  while stdin.readBinary(buf[end..]) {
    end = bufLen;
    bufLen += min(1024**2, bufLen);
    bufDom = {0..<bufLen};
  }
  end = stdin.offset()-1;

  // For some reason, we don't get a linefeed at the end... tack one on
  buf[end] = eol;

  // Dump the file back out to match against the input
  stdout.writeBinary(buf[0..end]);
}
