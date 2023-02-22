use FileSystem, IO;

var f1 = open("deprecatedSameFile.chpl", ioMode.r), f2 = open("deprecatedSameFile.chpl", ioMode.r);
writeln(sameFile(f1, f2));
