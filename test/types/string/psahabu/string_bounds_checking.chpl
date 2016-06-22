var s = "hello";
writeln(s);
var c = s[100];
writeln("index out of bounds did not halt");
c = s[-100..100];
writeln("range out of bounds did not halt");
