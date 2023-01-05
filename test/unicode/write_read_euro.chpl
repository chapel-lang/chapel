use IO;

var f = openTempFile();

var euro = new ioChar(0x20ac); // euro sign "?";
var got = new ioChar(0);

writeln("Writing ", euro);
var writer = f.writer();
writer.write(euro);
writer.close();

var reader = f.reader();
reader.read(got);
reader.close();

writeln("Read  ", got);

assert( got == euro );
