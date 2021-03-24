use OrderedMap;
use utilFunctions;



var m = new orderedMap(string, int, false, defaultComparator);

for i in 1..10 {
  m[intToEnglish(i)] = i;
}
writeln(m);
