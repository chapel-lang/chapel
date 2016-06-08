use MatrixMarket;
config const filename : string;

var o = mmreadsp(complex, filename);
writeln(o);

