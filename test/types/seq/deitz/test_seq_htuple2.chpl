var t1 : 2*integer = (1, 2);
var t2 : 2*integer = (3, 4);
var s : seq of 2*integer = (/ t1, t2 /);

writeln(t1);
writeln(t2);
writeln(s);

t1 = (5, 6);

writeln(t1);
writeln(t2);
writeln(s);
