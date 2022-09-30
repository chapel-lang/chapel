use IO;
use CTypes;

var a_unsigned : [0..<1032] uint(8);
var ch = openreader("./jab.txt");
const num_b_us = ch.readAll(a_unsigned);
ch.close();

writeln("unsigned:");
writeBytesArray(a_unsigned, num_b_us);

var a_signed: [0..<1032] int(8);
ch = openreader("./jab.txt");
const num_b_s = ch.readAll(a_signed);
ch.close();

writeln("\nsigned:");
writeBytesArray(a_signed, num_b_s);

proc writeBytesArray(a: [], nb: int) {
    const s = createStringWithBorrowedBuffer(c_ptrTo(a), length=nb, size=a.size);
    write(s);
}
