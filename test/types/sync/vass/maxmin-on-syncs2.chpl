var s1$: sync int = 1;
var s2$: sync int = 2;
var si1$: single int = 1;
var si2$: single int = 2;

writeln(min(s1$, s2$));
writeln(min(si1$, si2$));

s1$.writeEF(1);
s2$.writeEF(2);

writeln(max(s1$, s2$));
writeln(max(si1$, si2$));
