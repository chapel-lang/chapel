use IO;

config const fileName = "test.txt";

var f = open(fileName, ioMode.cwr);

var w = f.writer();

assert(!w.isclosed());
w.close();
assert(w.isclosed());
