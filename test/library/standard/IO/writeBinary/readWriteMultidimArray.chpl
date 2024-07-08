use IO, BlockDist;

config param useBlock = false;

var Dom = if useBlock then blockDist.createDomain({1..10, 1..10})
                      else {1..10, 1..10};

var A: [Dom] real = [(i,j) in Dom] i * 100 + j/100.0;

writeln(A);

var outfile = open("out.bin", ioMode.cw);
outfile.writer(locking=false).writeBinary(A);
outfile.close();

var B: [Dom] real;

var infile = open("out.bin", ioMode.r);
infile.reader(locking=false).readBinary(B);
infile.close();

writeln(B);

if + reduce (A == B) != Dom.size then
  halt("Mismatch between A and B");
else
  writeln("Matched!");
