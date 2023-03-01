use IO;

// testing deprecated variants of `fileReader.readBinary(a: [])` that return a `bool`

// try reading after fileReader is at EOF, should return false:
var a : [0..<8] uint(8),
    r = openReader("./readBinaryArrayInput.bin"),
    f = r.readAll();
writeln(r.readBinary(a));
writeln(a);

// read part of the file, should return 'true':
var b : [0..<4] uint(8);
writeln(openReader("./readBinaryArrayInput.bin").readBinary(b));
writeln(b);

// read the whole file, should return 'true':
var c : [0..<8] uint(8);
writeln(openReader("./readBinaryArrayInput.bin").readBinary(c));
writeln(c);

// ask for more than 8 bytes, should throw:
var d : [0..<10] uint(8);
try {
    openReader("./readBinaryArrayInput.bin").readBinary(d);
} catch e : UnexpectedEofError {
    writeln("caught: ", e);
} catch {
    writeln("wrong error type!");
}
