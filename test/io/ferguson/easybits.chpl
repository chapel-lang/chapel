
use FileSystem;
use IO;

config const testfile="test.bin";
var f = open(testfile, ioMode.cwr);

{
    var w = f.writer(kind=ionative);

    // Write 011
    w.writebits(0b011, 3);
    w.close();
}

// Try reading it back the way we wrote it.
{
    var r = f.reader(kind=ionative);
    var tmp:uint(64);

    r.readbits(tmp, 3);
    writeln("Read ", tmp);
    assert(tmp == 0b011);
}

FileSystem.remove(testfile);

