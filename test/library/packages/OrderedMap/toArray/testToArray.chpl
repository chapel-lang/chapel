use OrderedMap;



var m = new orderedMap(int, int, false, defaultComparator);

for i in -10..10 do
  m[i] = -i + 100;

var pairA = m.toArray(),
    keysA = m.keysToArray(),
    valsA = m.valuesToArray();

writeln(pairA);
writeln(keysA);
writeln(valsA);
