var s1 : seq of int = (/1, 2, 3, 4/);
var s2 : seq of int = (/5, 6, 7, 8/);

writeln(s1);
writeln(s2);

var ss : seq of seq of int = (/s1, s2/);

writeln(ss);
