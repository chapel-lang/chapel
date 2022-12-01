use IO;

var str:bytes;

// read 10 characters
stdin.readBytes(str, 10);

writeln("Read\n", str);

// read the rest of the file in
stdin.readBytes(str);

writeln("Read\n", str);
