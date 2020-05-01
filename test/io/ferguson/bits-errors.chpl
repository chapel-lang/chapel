// tests convenience signatures for bit-I/O routines
// these help avoid casting

use IO;

config const test=1;
config const testfile="test.bin";
var f = open(testfile, iomode.cwr);
{
  var w = f.writer(kind=ionative);

    // Write 011
    w.writebits(0b011, 3);
    w.close();
}

if test == 1 {
    var w = f.writer(kind=ionative);

    // test writing negative # of bits
    // Write 011
    var x = 0b011;
    var nbits = -1;
    w.writebits(x, nbits);
    w.close();
}

if test == 2 {
    var w = f.writer(kind=ionative);

    // test writing too many bits
    // Write 011
    var x:uint(8) = 0b011;
    var nbits = 9;
    w.writebits(x, nbits);
    w.close();
}


if test == 3 {
    var r = f.reader(kind=ionative);

    // test reading negative # of bits
    var tmp:int;
    var nbits = -1;
    // now tmp, nbits are int
    r.readbits(tmp, nbits);
    assert(tmp == 0b011);
}

if test == 4 {
    var r = f.reader(kind=ionative);

    // test reading too many bits
    var tmp:uint(8);
    var nbits = 9;
    // now tmp, nbits are int
    r.readbits(tmp, nbits);
    assert(tmp == 0b011);
}

unlink(testfile);

