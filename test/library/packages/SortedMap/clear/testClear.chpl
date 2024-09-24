use SortedMap;
use utilFunctions;



var m = new sortedMap(string, int, false, new DefaultComparator());

for i in 1..99 by 3 {
  m[intToEnglish(i)] = i;
}

m.clear();
writeln(m);
