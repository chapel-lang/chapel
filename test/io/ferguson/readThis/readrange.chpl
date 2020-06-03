use IO;

var f = openmem();

var w = f.writer();

var A = 1..10;

writeln(A);
w.write(A);

w.close();

var r = f.reader();

var B = 99..100;

r.read(B);

writeln(B);
