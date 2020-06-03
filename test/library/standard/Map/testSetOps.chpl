use Map;

var m1 = new map(int, int);
var m2 = new map(int, int);

m1[1] = 1;
m1[2] = 2;
m1[3] = 3;
m2[1] = -1;
m2[3] = -3;
m2[5] = -5;

writeln((m1 + m2).toArray().sorted());
writeln((m1 | m2).toArray().sorted());
writeln((m1 & m2).toArray().sorted());
writeln((m1 - m2).toArray().sorted());
writeln((m1 ^ m2).toArray().sorted());
