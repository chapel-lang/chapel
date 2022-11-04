use IO;

testBinaryRead("./input/tu8.bin", makeUnsignedArray(8));

testBinaryRead("./input/tu16.bin", makeUnsignedArray(16), ioendian.big);
testBinaryRead("./input/tu32.bin", makeUnsignedArray(32), ioendian.big);
testBinaryRead("./input/tu64.bin", makeUnsignedArray(64), ioendian.big);

testBinaryRead("./input/tu16le.bin", makeUnsignedArray(16), ioendian.little);
testBinaryRead("./input/tu32le.bin", makeUnsignedArray(32), ioendian.little);
testBinaryRead("./input/tu64le.bin", makeUnsignedArray(64), ioendian.little);

testBinaryRead("./input/ti8.bin", makeSignedArray(8));

testBinaryRead("./input/ti16.bin", makeSignedArray(16), ioendian.big);
testBinaryRead("./input/ti32.bin", makeSignedArray(32), ioendian.big);
testBinaryRead("./input/ti64.bin", makeSignedArray(64), ioendian.big);

testBinaryRead("./input/ti16le.bin", makeSignedArray(16), ioendian.little);
testBinaryRead("./input/ti32le.bin", makeSignedArray(32), ioendian.little);
testBinaryRead("./input/ti64le.bin", makeSignedArray(64), ioendian.little);

proc testBinaryRead(path: string, expected_values, endian: ioendian = ioendian.native) {
    var reader = openreader(path);
    var values : expected_values.type;

    try {
        var didRead = reader.readBinary(values, endian);
        writeln(didRead, "\t", (&& reduce (values == expected_values)));
    } catch e {
        writeln(e);
    }
}

// 8 => [1, 2, 4, 8, 16, 32, 64, 128]
proc makeUnsignedArray(param size = 8) {
    var a : [0..#size] uint(size);
    var x : uint(size) = 1;
    a[0] = x;

    for i in 1..#(size-1) {
        x *= 2;
        a[i] = x;
    }

    return a;
}

// 8 => [1, 2, 4, 8, 16, 32, 64]
proc makeSignedArray(param size = 8) {
    var a : [0..#(size-1)] int(size);
    var x : int(size) = 1;
    a[0] = x;

    for i in 1..#(size-2) {
        x *= 2;
        a[i] = x;
    }

    return a;
}
