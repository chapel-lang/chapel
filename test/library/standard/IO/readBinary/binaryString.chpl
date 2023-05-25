use IO;

var s: string;
try {
    var didRead = stdin.readBinary(s, 4);
    write(s);
} catch e {
    writeln(e);
}

stdin.matchLiteral(" very interesting");

try {
    var didRead = stdin.readBinary(s, 29);
    writeln(s);
} catch e {
    writeln(e);
}

// the channel is empty now
try {
    var didNotRead = ! stdin.readBinary(s, 1);
    writeln(didNotRead, "\t", s.isEmpty());
} catch e {
    writeln(e);
}
