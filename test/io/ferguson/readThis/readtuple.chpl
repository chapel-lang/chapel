use IO;

var f = openMemFile();

var w = f.writer(locking=false);

var A = (1,5,2);

writeln(A);
w.write(A);

w.close();

var r = f.reader(locking=false);

var B = (0,0,0);

r.read(B);

writeln(B);

