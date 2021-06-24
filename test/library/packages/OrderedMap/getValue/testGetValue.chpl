use OrderedMap;



class C {
  var i: int;
}

proc test() {
  var m = new orderedMap(string, shared C, false, defaultComparator);

  m.add("one", new shared C(1));
  m.add("two", new shared C(2));

  var x = m.getValue("one");
  x!.i = -1;

  writeln(m);
}
test();

