use OrderedMap;
use utilFunctions;

config param impl: mapImpl;

var m = new orderedMap(string, int, false, defaultComparator, impl);

for i in 1..10 {
  m[intToEnglish(i)] = i;
}
writeln(m);
