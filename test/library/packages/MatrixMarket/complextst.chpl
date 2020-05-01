use IO, MatrixMarket;
const filename : string = "complexsp.mtx";

var o = mmreadsp(complex, filename);
writeln(o);

mmwrite("cmplx.mtx", o);

var f = open("cmplx.mtx", iomode.r);
for l in f.lines() { writeln(l); }
