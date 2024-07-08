use IO, FileSystem;

var A = [1, 2, 3, 4, 5, 6];

var outfile = open("writeBinEmptyArray.bin", ioMode.cw).writer(locking=false);
outfile.writeBinary(A[1..0]);
outfile.close();

var infile = open("writeBinEmptyArray.bin", ioMode.r).reader(locking=false);
infile.readBinary(A[1..0]);
infile.close();

writeln(A);

remove("writeBinEmptyArray.bin");
