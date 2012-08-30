var D = {1..10,1..10};
var A: [D] real;
var value: real;
var Adat = open('Adata.dat', iomode.r).reader();
//new file('Adata.dat',path='./',mode=FileAccessMode.read);

for ij in D {
  Adat.read(value);
  A(ij) = value;
}
Adat.close();
writeln(A);
