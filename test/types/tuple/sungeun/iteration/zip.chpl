config var x = 1.0;
config var y = -1.0;
config var z = 83.0;

var A = [x, y, z];
var B = [y, z, x];
var C = [z, x, y];

for e in zip((x,y,z), (A,B,C)) do writeln(e);

var t1 = (x, y, z);
for e in zip(t1, (A,B,C)) do writeln(e);

var t2 = (A, B, C);
for e in zip((x,y,z), t2) do writeln(e);

for e in zip (t1, t2) do writeln(e);

for e in zip (t2, t1) do writeln(e);


