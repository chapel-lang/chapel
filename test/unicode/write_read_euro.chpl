use IO;

var f = openTempFile();

var euro = 0x20ac; // euro sign "?";
var got;

write("Writing ");
stdout.writeCodepoint(euro);
writeln();

var writer = f.writer();
writer.writeCodepoint(euro);
writer.close();

var reader = f.reader();
got = reader.readCodepoint();
reader.close();

write("Read  ");
stdout.writeCodepoint(got);
writeln();

assert( got == euro );
