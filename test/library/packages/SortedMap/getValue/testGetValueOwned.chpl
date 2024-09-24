use SortedMap;



class C {
  var i: int;
}

proc test() {
  var m = new sortedMap(string, owned C, false, new DefaultComparator());

  m.add("one", new owned C(1));
  m.add("two", new owned C(2));

  var x = m.getValue("one");
  x!.i = -1;

  writeln(m);
}
test();

