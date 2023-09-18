var s = "my string";
var d = string.createBorrowingBuffer(s);
writeln(d.numCodepoints);
writeln(d.size);
writeln(d.cachedNumCodepoints);
writeln(d.count("s"));
