use SortedMap;



class C {
  var i: int;
}

var e1 = new shared C(1);
var e2 = new shared C(2);

var m = new sortedMap(string, borrowed C, false, new DefaultComparator());

m.add("one", e1);
m.add("two", e2);

writeln(m);
