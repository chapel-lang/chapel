use IO;

var str:string;

// read 10 codepoints
stdin.readString(str, 10);

writeln("Read\n", str);

// read the rest of the file in
stdin.readString(str, 17);

writeln("Read\n", str);
