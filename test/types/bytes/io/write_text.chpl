use IO, ChplFormat;

var b = b"some\tbytes with € (the euro char)";
writeln("Direct print: ", b);
stdout.withSerializer(ChplSerializer).writef("Print repr: %?\n", b);
