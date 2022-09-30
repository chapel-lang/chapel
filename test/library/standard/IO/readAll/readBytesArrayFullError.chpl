use IO;
use OS;
use CTypes;

var a : [0..<100] uint(8),
    num_b: int = 0;

var ch = openreader("./jab.txt");
try {
    num_b = ch.readAll(a);
} catch e:IoError {
    writeln(e);
} catch {
    writeln("threw wrong error type");
}
const s_remaining = ch.readAll(string);
ch.close();

const s_from_array = createStringWithBorrowedBuffer(c_ptrTo(a), length=a.size, size=a.size);

write(s_from_array + s_remaining);
