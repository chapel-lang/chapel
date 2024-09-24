use SortedMap;
use utilFunctions;



var m = new sortedMap(string, int, false, new DefaultComparator());

for i in 1..10 {
  m[intToEnglish(i)] = i;
}
writeln(m);
