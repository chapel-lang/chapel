use IO;
use CTypes;

const NUMBYTES = 1032;

// array is larger than necessary to fit the whole file...
var a : [0..<1100] uint(8) = 0;
var ch = openReader("./jab.txt");
const num_b = ch.readAll(a);
ch.close();

// only the number of bytes in the file were read:
writeln(num_b == NUMBYTES);

// the remainder of the array is still zero:
writeln(&& reduce (a[(NUMBYTES+1)..] == 0));

// the file can be reproduced from the bytes array:
writeln(createStringWithBorrowedBuffer(c_ptrTo(a), length=num_b, size=num_b));
