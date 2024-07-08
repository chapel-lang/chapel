use IO;

var str:string;

// set stdin to binary mode
var style = stdin._styleInternal();
style.binary = 1;
stdin._set_styleInternal(style);

// read 10 codepoints
stdin.readString(str, 10);
writeln("Read\n", str);

// read the rest of the file in
stdin.readString(str, 17);
writeln("Read\n", str);
