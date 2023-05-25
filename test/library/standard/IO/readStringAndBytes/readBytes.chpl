use IO;

var f = openReader("./inputData.txt");

// check that both interfaces work:
const first_line = f.readBytes(23);
var second_line = b"";
f.readBytes(second_line, 38);
write(first_line, second_line);

// try to read more at EOF:
try {
    f.readBytes(1);
} catch e {
    writeln(e);
}
var empty = b"";
writeln(f.readBytes(empty, 1));
writeln(empty.isEmpty());
