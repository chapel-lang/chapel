var s1$: sync int = 1;
var n2: int = 2;
var si1$: single int = 1;
var ni2: int = 2;

writeln(min(s1$, n2));
writeln(min(si1$, ni2));

s1$.writeEF(1);

writeln(max(s1$, n2));
writeln(max(si1$, ni2));
