use IO;
use OS;

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
ch.close();

writeln(a);
writeln(num_b);
