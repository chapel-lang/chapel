use IO;
use CTypes;

var h : iohints;
h = IOHINT_RANDOM;
h = IOHINT_SEQUENTIAL;
h = IOHINT_CACHED;
h = IOHINT_PARALLEL;
h = IOHINT_NONE;

var f, c;

f = open("./iohints.chpl", iomode.r, hints = h); f.close();
openfd(0, hints = h);
openfp(chpl_cstdout(), hints = h);
opentmp(hints = h);
c = openreader("./iohints.chpl", hints = h); f.close();
