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

config var readSize = 16384, // how much to read at a time
           n = 0;            // a dummy variable to support the CLBG framework

proc main() {
  // read in the data using an incrementally growing buffer
  var bufSize = readSize,
      bufDom = {0..<bufSize},
      buf: [bufDom] uint(8),
      seqStart, totRead, end = 0;

  do {
    const start = end,
          more = stdinBin.read(buf[start..#readSize]);
    if more {
      totRead += readSize;
    } else {
      readSize = stdinBin.offset() - totRead + 1;
    }

    do {
      if end != 0 && buf[end] == '>'.toByte() {
        revcomp(buf, seqStart, end);
        seqStart = end;
      }
      end += 1;
    } while end < start+readSize;

    if more {
      // if we processed one or more sequences, shift leftovers down
      if seqStart != 0 {
        for (s,d) in zip(seqStart..<start+readSize, 0..) do
          buf[d] = buf[s];
        end -= seqStart;
        seqStart = 0;
      }

      // resize if necessary
      if end + readSize > bufSize {
        bufSize *= 2;
        bufDom = {0..<bufSize};
      }
    }
  } while more;
  revcomp(buf, seqStart, end-1);
}

proc revcomp(buf, in lo, in hi) {
  param eol  = '\n'.toByte(),      // end-of-line, as an integer
        // a 'bytes' value that stores the complement of each base at its index
        cmp = b"                                                             "
            + b"    TVGH  CD  M KN   YSAABW R       TVGH  CD  M KN   YSAABW R";
              //    ↑↑↑↑  ↑↑  ↑ ↑↑   ↑↑↑↑↑↑ ↑       ↑↑↑↑  ↑↑  ↑ ↑↑   ↑↑↑↑↑↑ ↑
              //    ABCDEFGHIJKLMNOPQRSTUVWXYZ      abcdefghijklmnopqrstuvwxyz

  if lo < hi {
    ref seq = buf[lo..<hi];

    // skip past header line
    while buf[lo] != eol {
      lo += 1;
    }

    while lo < hi {
      do {
        lo += 1;
      } while buf[lo] == eol;

      do {
        hi -= 1;
      } while buf[hi] == eol;

      if lo < hi {
        (buf[lo], buf[hi]) = (cmp(buf[hi]), cmp(buf[lo]));
      }
    }
    stdoutBin.write(seq);
  }
}
