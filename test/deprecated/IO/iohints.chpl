use IO;
use CTypes;

var h : iohints;
h = IOHINT_RANDOM;
h = IOHINT_SEQUENTIAL;
h = IOHINT_CACHED;
h = IOHINT_PARALLEL;
h = IOHINT_NONE;

var f, cr, cw;

f = open("./iohints.chpl", iomode.r, hints = h);
f.reader(hints = h);
f.close();

f = open("./iohints.chpl", iomode.rw, hints = h);
f.writer(hints = h);
f.close();

openfd(0, hints = h);
openfp(chpl_cstdout(), hints = h);
opentmp(hints = h);

cr = openreader("./iohints.chpl", hints = h); cr.close();
cw = openwriter("./blah.txt", hints = h); cw.close();
