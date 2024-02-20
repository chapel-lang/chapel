use IO;

testBinaryWrite("U8", makeUnsignedArray(8));
testBinaryWrite("I8", makeSignedArray(8));

testBinaryWrite("U16_BE", makeUnsignedArray(16), endianness.big);
testBinaryWrite("U32_BE", makeUnsignedArray(32), endianness.big);
testBinaryWrite("U64_BE", makeUnsignedArray(64), endianness.big);

testBinaryWrite("U16_LE", makeUnsignedArray(16), endianness.little);
testBinaryWrite("U32_LE", makeUnsignedArray(32), endianness.little);
testBinaryWrite("U64_LE", makeUnsignedArray(64), endianness.little);

testBinaryWrite("I16_BE", makeSignedArray(16), endianness.big);
testBinaryWrite("I32_BE", makeSignedArray(32), endianness.big);
testBinaryWrite("I64_BE", makeSignedArray(64), endianness.big);

testBinaryWrite("I16_LE", makeSignedArray(16), endianness.little);
testBinaryWrite("I32_LE", makeSignedArray(32), endianness.little);
testBinaryWrite("I64_LE", makeSignedArray(64), endianness.little);


proc testBinaryWrite(testName, values, endian: endianness = endianness.native) {
    var w = openWriter(testName + ".bin", locking=false);
    w.writeBinary(values, endian);
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
