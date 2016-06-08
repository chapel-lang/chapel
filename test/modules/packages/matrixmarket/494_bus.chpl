use MatrixMarket;
config const filename : string;

var o = mmreadsp(real, filename);
writeln(o);

