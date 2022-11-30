// Test that an UnexpectedEofError error is thrown if
//   the channel contains fewer bytes than requested

use IO;

var r = openreader("./input/tu8.bin");

try {
    // file only has 8 bytes
    var expected : [0..#9] uint(8);

    var didRead = r.readBinary(expected);
} catch e: UnexpectedEofError {
    writeln(e);
} catch e {
    writeln("threw wrong error type: ", e);
}
