var D1: domain(int);
var D2: domain(int);
var A1: [D1] int;
var A2: [D2] int;

D1 += 1; D1 += 2; D1 += 3;
D2 += 1; D2 += 3; D2 += 5;

A1[1] = 1; A1[2] = 2; A1[3] = 3;

A2[1] = -1; A2[3] = -3; A2[5] = -5;

writeln((A1 + A2).sorted());
writeln((A1 | A2).sorted());
writeln((A1 & A2).sorted());
writeln((A1 - A2).sorted());
writeln((A1 ^ A2).sorted());
