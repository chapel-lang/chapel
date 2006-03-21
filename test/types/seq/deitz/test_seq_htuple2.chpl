var t1 : 2*int = (1, 2);
var t2 : 2*int = (3, 4);
var s : seq of 2*int = (/ t1, t2 /);

writeln(t1);
writeln(t2);
writeln(s);

t1 = (5, 6);

writeln(t1);
writeln(t2);
writeln(s);
