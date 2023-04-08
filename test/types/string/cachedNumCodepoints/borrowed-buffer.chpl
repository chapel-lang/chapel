var s = "my string";
var d = string.createWithBorrowedBuffer(s);
writeln(d.numCodepoints);
writeln(d.size);
writeln(d.cachedNumCodepoints);
writeln(d.count("s"));