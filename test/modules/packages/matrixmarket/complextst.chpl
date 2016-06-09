use MatrixMarket;
const filename : string = "complexsp.mtx";

var o = mmreadsp(complex, filename);
writeln(o);

