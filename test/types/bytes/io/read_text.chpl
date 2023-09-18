use IO, ChplFormat;

var b: bytes;
readln(b);
writeln("You have entered: ", b);
stdin.withDeserializer(chplDeserializer).readf("%?", b);
writeln("You have entered: ", b);
