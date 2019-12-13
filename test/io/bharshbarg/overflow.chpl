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
assert(!r.readline(data, numRead, start = lineLen * 2));
assert(!r.readline(data, numRead, start = -10));

// Can't read more than the array can store
assert(!r.readline(data, numRead, amount = lineLen * 2));
assert(!r.readline(data, numRead, start = lineLen / 2, amount=lineLen));

// Make sure we don't read more bytes than there is room in the array.
r.readline(data, numRead, start=lineLen / 2);
assert(numRead == lineLen + 1 - (lineLen / 2) + 1);
