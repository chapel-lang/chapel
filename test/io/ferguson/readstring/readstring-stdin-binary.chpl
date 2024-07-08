use IO;

var str:string;

// set stdin to binary mode
var style = stdin._styleInternal();
style.binary = 1;
stdin._set_styleInternal(style);

// read 21 bytes
stdin.readString(str, 21);
writeln("Read\n", str);
