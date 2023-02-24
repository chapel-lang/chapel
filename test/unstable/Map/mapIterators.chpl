use Map;

var m = new map(int, int);
m[0] = 1;

for key in m do
  writeln(key);

for (key, val) in m.items() do
  writeln(key, "->", val);
