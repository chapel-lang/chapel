use MatrixMarket;
use IO;
const filename : string = "complexsp.mtx";

var o = mmreadsp(complex, filename);

mmwrite("complextest.mtx", o);

var r = openreader("complextest.mtx");
for l in r.lines() { writeln(l); }
