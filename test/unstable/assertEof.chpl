use IO;

var fr = openReader("assertEof.chpl");
fr.readAll();

fr.assertEOF();
