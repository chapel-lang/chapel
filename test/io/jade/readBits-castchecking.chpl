use FileSystem;
use IO;

config const testfile="test.bin";
var f = open(testfile, iomode.cwr);

var w = f.writer(kind=ionative);
var b = [0xaa66, 0x7852, 0x9287, 0x1455, 0x2299, 0x7218, 0x8278, 0x4082] :uint(16);
w.writeBinary(b);

var r = f.reader(kind=ionative);
var tmp:uint(8);

try {
    r.readBits(tmp, 0); // should not throw
    writeln("Correctly passes on 0 bits");
} catch e {
    writeln("Incorrectly throws on 0 bits - ", e);
}

try {
    r.readBits(tmp, 4); // should not throw
    writeln("Correctly passes on 4 bits");
} catch e {
    writeln("Incorrectly throws on 4 bits - ", e);
}

try {
    r.readBits(tmp, 7:uint); // should not throw
    writeln("Correctly passes on 7 bits");
} catch e {
    writeln("Incorrectly throws on 7 bits - ", e);
}

try {
    r.readBits(tmp, 8:uint); // should not throw
    writeln("Correctly passes on 8 bits");
} catch e {
    writeln("Incorrectly throws on 8 bits - ", e);
}

try {
    r.readBits(tmp, 9); // should throw
    writeln("Incorrectly passes on 9 bits");
} catch e {
    writeln("Correctly throws on 9 bits - ", e);
}

try {
    r.readBits(tmp, -1); // should throw
    writeln("Incorrectly passes on -1 bits");
} catch e {
    writeln("Correctly throws on -1 bits - ", e);
}

FileSystem.remove(testfile);
