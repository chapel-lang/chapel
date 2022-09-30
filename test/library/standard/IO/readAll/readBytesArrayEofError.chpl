use IO;
use OS;

var a : [0..<1024] uint(8),
    num_b: int = 0;

var ch = openreader("./jab.txt");
try {
    num_b = ch.readAll(a);
} catch e:EofError {
    writeln(e);
} catch {
    writeln("wrong error type");
}
ch.close();

writeln(num_b);
