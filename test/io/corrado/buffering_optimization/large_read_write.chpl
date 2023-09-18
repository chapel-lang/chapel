use IO, CTypes, FileSystem;

config const size = 1024: int(64);

const w = openWriter("./x.bin", locking=false, hints=ioHintSet.mmap(false));
var a = [i in 0..<size] i;
w.writeBinary(a);
w.write("yep");
w.close();

const r = openReader("./x.bin", locking=false, hints=ioHintSet.mmap(false));
var b : [0..<size] int(64);
r.readBinary(b);

writeln(&& reduce (a == b));
writeln(r.read(string));

remove("x.bin");
