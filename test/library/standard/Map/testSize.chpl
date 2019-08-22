use Map;
use utilFunctions;

var m = new map(string, real);

writeln(m.isEmpty());
writeln(m.size);

for i in 1..99 by 4 do
  m[intToEnglish(i)] = i: real;

writeln(m.isEmpty());
writeln(m.size);
