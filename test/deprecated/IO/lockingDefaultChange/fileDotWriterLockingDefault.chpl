use IO;
var f = open("fileDotWriterOut.txt", ioMode.cwr);

// should trigger a warning, locking isn't specified
var r = f.writer();
r.writeln("Hello, World!");

// shouldn't trigger a warning, locking is specified explicitly
var r2 = f.reader(locking=false);
r.writeln("Hello, World!");
