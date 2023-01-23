use IO;

var f = openreader("./inputData.txt");

// check that both interfaces work:
const first_line = f.readBytes(23);
var second_line = b"";
f.readBytes(second_line, 38);
write(first_line, second_line);

// try to read more after EOF:
const should_be_empty = f.readBytes(1);
var also_emtpy = b"";
const did_read = f.readBytes(also_emtpy, 1);
writeln(should_be_empty.isEmpty());
writeln(also_emtpy.isEmpty());
writeln(did_read);
