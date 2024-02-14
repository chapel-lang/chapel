use IO;

var f = openTempFile();

var w = f.writer(locking=false);

var A = for i in 1..10 do i;

writeln(A);
w.write(A);

w.close();

var r = f.reader(locking=false);

var B = for i in 1..10 do 2*i;

r.read(B);

writeln(B);

