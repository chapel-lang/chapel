use IO, MatrixMarket;
const filename : string = "complexsp.mtx";

var o = mmreadsp(complex, filename);
writeln(o);

mmwrite("cmplx.mtx", o);

var r = openReader("cmplx.mtx", locking=false);
for l in r.lines() { writeln(l); }
