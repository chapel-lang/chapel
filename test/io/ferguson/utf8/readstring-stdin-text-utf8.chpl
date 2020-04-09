use IO;

var str:string;

// read 10 characters
stdin.readstring(str, 10);

writeln("Read\n", str);

// read the rest of the file in
stdin.readstring(str);

writeln("Read\n", str);

