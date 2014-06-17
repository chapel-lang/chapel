extern proc sizeof(x): int(64);

//var b  : bool     = true;  NOT PORTABLE
var b8 : bool(8)  = true;
var b16: bool(16) = true;
var b32: bool(32) = true;
var b64: bool(64) = true;

//writeln("sizeof(b) = ", sizeof(b));
writeln("sizeof(b8) = ", sizeof(b8));
writeln("sizeof(b16) = ", sizeof(b16));
writeln("sizeof(b32) = ", sizeof(b32));
writeln("sizeof(b64) = ", sizeof(b64));
