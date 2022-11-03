use IO;

var b: bytes;
try {
    var didRead = stdin.readBinary(b, 4);
    write(b);
} catch e {
    writeln(e);
}

stdin.matchLiteral(" very interesting");

try {
    var didRead = stdin.readBinary(b, 29);
    write(b);
} catch e {
    writeln(e);
}

// the channel is empty now
try {
    var didNotRead = ! stdin.readBinary(b, 1);
    writeln(didNotRead);
} catch e {
    writeln(e);
}
