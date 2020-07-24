use OrderedMap;

var m1 = new orderedMap(int, int);
var m2 = new orderedMap(int, int);

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
