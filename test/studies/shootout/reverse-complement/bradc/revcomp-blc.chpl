/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Brad Chamberlain
   based on the C gcc #5 version by Mr Ledrug
   and the Chapel #2 version by myself and Ben Harshbarger
*/

use IO;

param eol = "\n".toByte();      // end-of-line, as an integer
//             01234567890
param cmpl = b"          \n                                                 "
            //      ABCDEFGHIJKLMNOPQRSTUVWXYZ      abcdefghijklmnopqrstuvwxyz
           + b"     TVGH  CD  M KN   YSAABW R       TVGH  CD  M KN   YSAABW R";

proc main(args: []) {
  const stdinBin = openfd(0).reader(iokind.native, locking=false,
                                 hints = QIO_CH_ALWAYS_UNBUFFERED),
        stdoutBin = openfd(1).writer(iokind.native, locking=false,
                                  hints=QIO_CH_ALWAYS_UNBUFFERED);

  // read in the data using an incrementally growing buffer
  var bufLen = 8 * 1024,
      bufDom = {0..<bufLen},
      buf: [bufDom] uint(8),
      end = 0;

  while true {
    if !stdinBin.read(buf[end..]) then break;
    end = bufLen;
    bufLen += min(1024**2, bufLen);
    bufDom = {0..<bufLen};
  }
  end = stdinBin.offset()-1;

  // process the buffer a sequence at a time, working from the end
  var hi = end;
  while (hi >= 0) {
    // search for the '>' that marks the start of a sequence
    var lo = hi;
    while buf[lo] != '>'.toByte() do
      lo -= 1;

    // reverse and complement the sequence once we find it
    revcomp(buf, lo, hi);

    hi = lo - 1;
  }

  // write out the transformed buffer
  stdoutBin.write(buf[..end]);
}


proc revcomp(buf, start, hi) {
  param cols = 61;  // the number of characters per full row (including '\n')
  var lo = start;

  // skip past header line
  do {
    lo += 1;
  } while buf[lo-1] != eol;

  // shift all of the linefeeds into the right places
  const len = hi - lo + 1,
        off = (len - 1) % cols,
        shift = cols - off - 1;

  if off {
    forall m in lo+off..<hi by cols {
      for i in m..#shift by -1 do
        buf[i+1] = buf[i];
      buf[m] = eol;
    }
  }

  // walk from both ends of the sequence, complementing and swapping
  forall (i,j) in zip(lo..#(len/2), ..<hi by -1) do
    (buf[i], buf[j]) = (cmpl[buf[j]], cmpl[buf[i]]);
}
