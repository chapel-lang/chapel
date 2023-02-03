use FileSystem, IO;

var f1 = open("deprecatedSameFile.chpl", iomode.r), f2 = open("deprecatedSameFile.chpl", iomode.r);
writeln(sameFile(f1, f2));
