use IO;

var str:bytes;

// read 10 characters
stdin.readbytes(str, 10);

writeln("Read\n", str);

// read the rest of the file in
stdin.readbytes(str);

writeln("Read\n", str);

