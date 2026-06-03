use Map;

// creating a module-scope map and iterating over it works on locale 0 or 1
var m: map(int, string);
m.add(1, "one");

for k in m.keys() do
  writeln(k);

for v in m.values() do
  writeln(v);

on Locales.last {
  for k in m.keys() do
    writeln(k);

  for v in m.values() do
    writeln(v);
}
