use IO;

config const lineLen = 100;

var f = openmem();
var w = f.writer();

for i in 1..lineLen do w.write(i%10);
w.writeln();

var r = f.reader();
var data : [1..lineLen + 1] uint(8);

var numRead : int;

// Can't start reading past the domain
assert(0 == r.readLine(data[lineLen * 2..]));
assert(0 == r.readLine(data[-10..]));

// Can't read more than the array can store
assert(0 == r.readLine(data, maxSize = lineLen * 2));
assert(0 == r.readLine(data[lineLen / 2..], maxSize=lineLen));

// Make sure we don't read more bytes than there is room in the array.
numRead = r.readLine(data[lineLen / 2..]);
assert(numRead == lineLen + 1 - (lineLen / 2) + 1);
