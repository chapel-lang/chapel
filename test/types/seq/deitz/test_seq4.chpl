var s1 : seq of string = (/ 3, 1, 4, 1, 5, 9 /);
var s2 : seq of string;

writeln(s1);
s2 = s1;
s1 = s1 # 2;
writeln(s2);
writeln(s1);
