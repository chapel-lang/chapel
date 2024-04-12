use IO;

var f = openReader("./inputData.txt", locking=false);

// check that both interfaces work:
const first_line = f.readString(23);
assert(first_line.size == 23);
var second_line = "";
f.readString(second_line, 35);
write(first_line, second_line);

// try to read more at EOF:
try {
    f.readString(1);
} catch e {
    writeln(e);
}
var empty = "";
writeln(f.readString(empty, 1));
writeln(empty.isEmpty());
