// There's no 'new' version of this test because the updated behavior of
// read/writeBinary is to disallow non-DR arrays.
//
// This test just serves to preserve the "normal" behavior of the deprecated
// readBinary for arrays. Once the deprecated method is no longer present,
// this test can be removed.

use IO, BlockDist;

var Dom = blockDist.createDomain({1..10});

var A: [Dom] real = [i in Dom] i;

var infile = open("out.bin", ioMode.r);
infile.reader().readBinary(A);
infile.close();

writeln(A);
