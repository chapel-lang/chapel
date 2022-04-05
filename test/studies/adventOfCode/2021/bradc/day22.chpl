use IO;

var D = {-50..50, -50..50, -50..50};
var A: [D] bool;

var val: string;
var xlo, xhi, ylo, yhi, zlo, zhi: int;
while readf("%s x=%i..%i,y=%i..%i,z=%i..%i", val, xlo, xhi, ylo, yhi, zlo, zhi) {
  writeln("Got: ", (val, xlo, xhi, ylo, yhi, zlo, zhi));
  const setting = val == "on";
  const SubD = D[xlo..xhi, ylo..yhi, zlo..zhi];
  A[SubD] = setting;
}
writeln(+ reduce A);
