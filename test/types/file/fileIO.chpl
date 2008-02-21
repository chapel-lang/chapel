config var n = 10,
           filename = "arr.out";

const ADom = [1..n, 1..n];

var A: [(i,j) in ADom] real = (i-1) + ((j-1)/10.0);

writeArray(n, A, filename);
var B = readArray(filename);

const numErrors = + reduce [i in ADom] (A(i) != B(i));

writeln("B is:\n", B);

if (numErrors > 0) {
  writeln("FAILURE");
} else {
  writeln("SUCCESS");
}


def writeArray(n, X, filename) {
  var outfile = new file(filename, FileAccessMode.write);
  outfile.open();
  outfile.writeln(n, " ", n);
  outfile.writeln(X);
  outfile.close();
}


def readArray(filename) {
  var m, n: int;

  var infile = new file(filename, FileAccessMode.read);
  infile.open();
  infile.read(m, n);

  const XDom = [1..m, 1..n];
  var X: [XDom] real;

  infile.read(X);

  infile.close();

  return X;
}


