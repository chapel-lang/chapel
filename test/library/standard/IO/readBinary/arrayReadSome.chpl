// Test that other binary read operations pick up where the array
//  interface leaves off when only part of the channel is read
//  using readBinary(ref a: [] numeric)

use IO;

var r = openreader("./input/tu8.bin");

var expected : [0..#7] uint(8);
var didRead = r.readBinary(expected);
writeln(didRead, "\t", expected);

var next : uint(8);
didRead = r.readBinary(next);
writeln(didRead, "\t", next);

var incorrectlyExpectingMore: [0..#1] uint(8);
try {
    // nothing more to read, should return false rather than throwing
    var didntRead = ! r.readBinary(incorrectlyExpectingMore);
    writeln(didntRead);
} catch e {
    writeln("should throw: ", e);
}
