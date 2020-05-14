/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Brad Chamberlain
   based on the C gcc #5 version by Mr Ledrug
*/

param eol = "\n".toByte();      // end-of-line, as an integer

const table = createTable();    // create the table of code complements

proc main(args: [] string) {
  use IO;
  const stdinBin = openfd(0).reader(iokind.native, locking=false,
                                 hints = QIO_CH_ALWAYS_UNBUFFERED),
        stdoutBin = openfd(1).writer(iokind.native, locking=false,
                                  hints=QIO_CH_ALWAYS_UNBUFFERED);;

  var bufLen = 8 * 1024,
      bufDom = {0..<bufLen},
      buf: [bufDom] uint(8),
      end = 0;

  do {
    const more = stdinBin.read(buf[end..]);
    if more {

      end = bufLen;
      bufLen += min(1024**2, bufLen);
      bufDom = {0..<bufLen};
    }
  } while more;

  for i in buf.indices by -1 {
    if buf[i] != 0 {
      end = i;
      break;
    }
  }

  if end {
    var to = end;

    do {
      var from = to;
      while buf[from] != '>'.toByte() do
        from -= 1;

      process(buf, from, to);

      to = from - 1;
    } while (to >= 0);

    stdoutBin.write(buf[..end]);
  }
}

proc process(buf, in from, in to) {
  do {
    from += 1;
  } while buf[from-1] != eol;

  const len = to - from,
        off = 60 - (len % 61);

  if off {
    for m in from+60-off..<to by 61 {
      for i in m..#off by -1 do
        buf[i+1] = buf[i];
      buf[m] = eol;
    }
  }

  to -= 1;
  do {
    ref d1 = buf[from],
        d2 = buf[to];

    (d1, d2) = (table[d2], table[d1]);

    from += 1;
    to -= 1;
  } while (from <= to);
}


proc createTable() {
  // `pairs` compactly represents the table we're creating, where the
  // first byte of each pair (in either case) maps to the second:
  //   A|a -> T, C|c -> G, G|g -> C, T|t -> A, etc.
  param pairs = b"ATCGGCTAUAMKRYWWSSYRKMVBHDDHBVNN",
        upperToLower = "a".toByte() - "A".toByte();

  var table: [0..127] uint(8);

  for i in pairs.indices by 2 {
    const src = pairs[i],
          dst = pairs[i+1];

    table[src] = dst;
    table[src+upperToLower] = dst;
  }
  table[eol] = eol;

  return table;
}
