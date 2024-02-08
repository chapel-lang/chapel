use IO;
var w = openWriter("openWriterLockingOut.txt");
w.writeln("Hello, World!");

// this shouldn't produce a warning because locking is set explicitly
var w2 = openWriter("openWriterLockingOut.txt", locking=false);
w.writeln("Hello, World!");
