use OrderedMap;
use utilFunctions;

config param impl: mapImpl;

var m = new orderedMap(string, int, false, defaultComparator, impl);

for i in 1..99 by 3 {
  m[intToEnglish(i)] = i;
}

m.clear();
writeln(m);
