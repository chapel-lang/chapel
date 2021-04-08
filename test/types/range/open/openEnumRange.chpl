enum color { red, green, blue };

for c in color.red..<color.blue do
  writeln(c);

for c in (color.green..color.blue)[..<color.blue] do
  writeln(c);
