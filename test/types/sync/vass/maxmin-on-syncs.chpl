var s1: sync int = 1;
var s2: sync int = 2;

writeln(min(s1.readFE(), s2.readFE()));

s1.writeEF(1);
s2.writeEF(2);

writeln(max(s1.readFE(), s2.readFE()));
