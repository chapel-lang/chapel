
use SortedMap;

record C {
  var i: int;
}

proc test() {
  var m = new sortedMap(string, C, false, new DefaultComparator());

  m.add("one", new C(1));
  m.add("two", new C(2));

  var x = m.getValue("one");
  x.i = -1;

  writeln(m);
}
test();

