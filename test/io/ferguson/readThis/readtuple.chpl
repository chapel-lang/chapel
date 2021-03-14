use IO;

var f = openmem();

var w = f.writer();

var A = (1,5,2);

writeln(A);
w.write(A);

w.close();

var r = f.reader();

var B = (0,0,0);

r.read(B);

writeln(B);

