var D = [1..10,1..10];
var A: [D] real;
var value: real;
var Adat = new file('Adata.dat',path='./',mode='r');

Adat.open();
for ij in D {
  Adat.read(value);
  A(ij) = value;
}
Adat.close();
writeln(A);
