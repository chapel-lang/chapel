use OrderedMap;

config param impl: mapImpl;

var m = new orderedMap(int, [1..2] int, false, defaultComparator, impl);

var A: [1..2] int = 1..2;
m.add(1, A);
//writeln(m);
