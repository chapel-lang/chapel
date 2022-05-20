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

var seqToPrint: atomic int = 0;

proc main() {
  // read in the data using an incrementally growing buffer
  var bufSize = readSize,
      bufDom = {0..<bufSize},
      buf: [bufDom] uint(8),
      seqStart, nextSeqID, totRead, end = 0;

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
        const ref seq = buf[seqStart..<end];
        begin with (in seq)
          revcomp(nextSeqID, seq);
        nextSeqID += 1;
        seqStart = end;
      }
      end += 1;
    } while end < start+readSize;

    if more {
      // if we processed one or more sequences, shift leftovers down
      if seqStart != 0 {
        // TODO: Cool to have a cool array shift that was parallel?
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
  revcomp(nextSeqID, buf[seqStart..<end-1]);
}

proc revcomp(seqID, seq:[?D]) {
  param eol  = '\n'.toByte(),      // end-of-line, as an integer
        cmp = b"                                                             "
            + b"    TVGH  CD  M KN   YSAABW R       TVGH  CD  M KN   YSAABW R";
              //    ↑↑↑↑  ↑↑  ↑ ↑↑   ↑↑↑↑↑↑ ↑       ↑↑↑↑  ↑↑  ↑ ↑↑   ↑↑↑↑↑↑ ↑
              //    ABCDEFGHIJKLMNOPQRSTUVWXYZ      abcdefghijklmnopqrstuvwxyz

  var lo = D.low,
      hi = D.high+1;

  if lo < hi {
    // skip past header line
    while seq[lo] != eol {
      lo += 1;
    }

    while lo < hi {
      do {
        lo += 1;
      } while seq[lo] == eol;

      do {
        hi -= 1;
      } while seq[hi] == eol;

      if lo < hi {
        (seq[lo], seq[hi]) = (cmp(seq[hi]), cmp(seq[lo]));
      }
    }
    seqToPrint.waitFor(seqID);
    stdoutBin.write(seq);
    seqToPrint.add(1);
  }
}
