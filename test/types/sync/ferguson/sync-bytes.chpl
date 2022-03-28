var x: sync bytes;

x.writeEF(b"hi "*23);

writeln(x.readFE());
