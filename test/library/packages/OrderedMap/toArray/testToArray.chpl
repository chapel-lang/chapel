use OrderedMap;

config param impl: mapImpl;

var m = new orderedMap(int, int, false, defaultComparator, impl);

for i in -10..10 do
  m[i] = -i;

var pairA = m.toArray(),
    keysA = m.keysToArray(),
    valsA = m.valuesToArray();

writeln(pairA.sorted());
writeln(keysA.sorted());
writeln(valsA.sorted());
