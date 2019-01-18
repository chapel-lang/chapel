config const n = 99;
var r = 2..n, P: [r] bool = true;
for i in r do if P[i] { P[i+i..by i] = false; }
for i in r do if P[i] { writeln(i); }