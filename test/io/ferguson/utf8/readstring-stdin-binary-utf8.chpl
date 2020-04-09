use IO;

var str:string;

// set stdin to binary mode
var style = stdin._style();
style.binary = 1;
stdin._set_style(style);

// read 10 bytes
stdin.readstring(str, 10);
writeln("Read\n", str);

// read the rest of the file in
stdin.readstring(str);
writeln("Read\n", str);

