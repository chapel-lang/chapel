use SortedMap;

record C {
  var i: int;
}

var m = new sortedMap(string, C, false, new DefaultComparator());

m.add("one", new C(1));
m.add("two", new C(2));

ref x = m.getReference("one");
x.i = -1;

writeln(m);
