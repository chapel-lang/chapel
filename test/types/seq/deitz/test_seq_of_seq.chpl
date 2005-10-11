var s1 : seq of integer = (/1, 2, 3, 4/);
var s2 : seq of integer = (/5, 6, 7, 8/);

writeln(s1);
writeln(s2);

var ss : seq of seq of integer = (/s1, s2/);

writeln(ss);
