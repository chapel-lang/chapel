use Vector;
var v:vector(int) = 1..8;
v.insert(2, 0);
writeln(v);
writeln(v.size);
v.insert(v.size, 0);
writeln(v);
writeln(v.size);
