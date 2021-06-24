use OrderedMap;
use utilFunctions;



var m = new orderedMap(string, real, false, defaultComparator);

writeln(m.isEmpty());
writeln(m.size);

for i in 1..99 by 4 do
  m[intToEnglish(i)] = i: real;

writeln(m.isEmpty());
writeln(m.size);
