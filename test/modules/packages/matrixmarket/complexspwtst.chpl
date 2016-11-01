use MatrixMarket;
use IO;
const filename : string = "complexsp.mtx";

var o = mmreadsp(complex, filename);

mmwrite("complextest.mtx", o);

var fd = open("complextest.mtx", iomode.r);
for l in fd.lines() { writeln(l); }

