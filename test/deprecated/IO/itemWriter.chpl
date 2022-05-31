use IO;

var fwiw = stdout.itemWriter(int);
fwiw.write(1);
fwiw.write(2);
fwiw.write(3);

var iw = new ItemWriter(int, stdout.kind, stdout.locking, stdout);
iw.write(4);
iw.write(5);
iw.write(6);
