use IO;

testBinaryRead("t8.txt", makeNumericArray(8));
testBinaryRead("t16.txt", makeNumericArray(16));
// testBinaryRead("t32.txt", makeNumericArray(32));
// testBinaryRead("t64.txt", makeNumericArray(64));

proc testBinaryRead(path: string, values) {
    var reader = openreader(path);

    writeln(values);
    writeln(values.type:string);

    try {
        var didRead = reader.readBinary(values);
        writeln(didRead);
    } catch e {
        writeln(e);
    }
}

proc makeNumericArray(param size = 8) {
    var a : [0..#size] uint(size);
    var x : uint(size) = 1;

    for i in 0..#size {
        a[i] = x;
        x *= 2;
    }

    return a;
}
