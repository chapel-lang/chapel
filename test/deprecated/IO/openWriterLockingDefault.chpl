use IO;
var w = openWriter("openReaderLockingOut.txt");
w.writeln("Hello, World!");

// this shouldn't produce a warning because locking is set explicitly
var w2 = openWriter("openReaderLockingOut.txt", locking=false);
w.writeln("Hello, World!");
