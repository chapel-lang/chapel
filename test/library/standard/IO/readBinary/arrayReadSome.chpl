// Test behavior of readBinary, when array size does not match the number of bytes in the file

use IO;

var r = openReader("./input/tu8.bin");

var partial: [0..#3] uint(8),
    larger: [0..#10] uint(8) = 1,
    noneLeft: [0..#5] uint(8) = 2;

// read some of the bytes, should fill the array and return partial.size
writeln(r.readBinary(partial), "\t", partial);

// read the rest of the bytes, should read the first 5 bytes,
//  return 5, and leave the remainder of the array unchanged
writeln(r.readBinary(larger), "\t", larger);

// try reading when at EOF, should return zero and leave 'noneLeft' unchanged
writeln(r.readBinary(noneLeft), "\t", noneLeft);
