use SortedMap;



class C {
  var i: int;
}

var m = new sortedMap(string, shared C, false, new DefaultComparator());

m.add("one", new shared C(1));
m.add("two", new shared C(2));

for v in m.values() {
  writeln(v);
}

