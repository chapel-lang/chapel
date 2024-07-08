use IO;

var f = openMemFile();

var w = f.writer(locking=false);

var A = {1..10};

writeln(A);
w.write(A);

w.close();

var r = f.reader(locking=false);

var B = {99..100};

r.read(B);

writeln(B);

