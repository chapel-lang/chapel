use SortedMap;



var m = new sortedMap(int, int, false, new DefaultComparator());

for i in -10..10 do
  m[i] = -i + 100;

var pairA = m.toArray(),
    keysA = m.keysToArray(),
    valsA = m.valuesToArray();

writeln(pairA);
writeln(keysA);
writeln(valsA);
