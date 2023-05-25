use IO;

// should generate 1 deprecation message
var m: iomode;

// should generate 2 deprecation messages
var f1 = open("test.txt", iomode.cwr, style=defaultIOStyleInternal());
f1.close();

// should generate 1 deprecation message
var f2 = open("test.txt", m);
f2.close();
