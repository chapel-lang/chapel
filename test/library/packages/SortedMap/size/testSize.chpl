use SortedMap;
use utilFunctions;



var m = new sortedMap(string, real, false, new DefaultComparator());

writeln(m.isEmpty());
writeln(m.size);

for i in 1..99 by 4 do
  m[intToEnglish(i)] = i: real;

writeln(m.isEmpty());
writeln(m.size);
