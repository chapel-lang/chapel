use IO;

config const delim = "!";
assert(delim.numBytes == 1);
config const lineLen = 100;

var f = openmem();
var w = f.writer();

for i in 1..lineLen-1 do w.write((i%9) + 1);

w.write(delim);

var r = f.reader();
var data : string;
r.readUntil(data, delim.toByte());
