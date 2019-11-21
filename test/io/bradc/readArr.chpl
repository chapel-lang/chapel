use IO;

config const filename = "Arr.dat";

var infile = open(filename, iomode.r);
var reader = infile.reader();

var (numElems, A) = readArray(reader);
var (numElems2, B) = readArray(reader);

writeln("Got array A with ", numElems, " elements: ", A);
writeln("Got array B with ", numElems2, " elements: ", B);

proc readArray(reader) {
  const arrSize = reader.read(int);
  var A: [0..#arrSize] real;

  for a in A do
    reader.read(a);
  
  return (arrSize, A);
}
