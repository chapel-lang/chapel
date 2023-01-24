use IO;

var f = openreader("./inputData.txt");

// check that both interfaces work:
const first_line = f.readString(23);
var second_line = "";
f.readString(second_line, 35);
write(first_line, second_line);

// try to read more after EOF:
const should_be_empty = f.readString(1);
var also_emtpy = "";
const did_read = f.readString(also_emtpy, 1);
writeln(should_be_empty.isEmpty());
writeln(also_emtpy.isEmpty());
writeln(did_read);
