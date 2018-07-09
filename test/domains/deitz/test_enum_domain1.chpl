enum color { red, green, blue };

var E: domain(color) = color.red..color.blue;

for i in zip(E,E,E) do
  writeln(i);
