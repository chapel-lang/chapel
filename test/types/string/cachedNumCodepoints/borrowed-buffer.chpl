var s = "my string";
var d = createStringWithBorrowedBuffer(s);
writeln(d.numCodepoints);
writeln(d.size);
writeln(d.cachedNumCodepoints);
writeln(d.count("s"));