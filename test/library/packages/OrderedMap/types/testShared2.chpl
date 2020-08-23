use OrderedMap;



class C {
  var i: int;
}

var m = new orderedMap(string, shared C, false, defaultComparator);

m.add("one", new shared C(1));
m.add("two", new shared C(2));

for v in m.values() {
  writeln(v);
}

