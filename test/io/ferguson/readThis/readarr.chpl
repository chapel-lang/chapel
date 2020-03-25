use IO;

var f = opentmp();

var w = f.writer();

var A = for i in 1..10 do i;

writeln(A);
w.write(A);

w.close();

var r = f.reader();

var B = for i in 1..10 do 2*i;

r.read(B);

writeln(B);

