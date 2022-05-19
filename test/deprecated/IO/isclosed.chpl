use IO;

config const fileName = "test.txt";

var f = open(fileName, iomode.cwr);

var w = f.writer();

assert(!w.isclosed());
w.close();
assert(w.isclosed());
