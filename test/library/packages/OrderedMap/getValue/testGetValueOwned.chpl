use OrderedMap;

config param impl: mapImpl;

class C {
  var i: int;
}

proc test() {
  var m= new orderedMap(string, owned C, false, defaultComparator, impl);

  m.add("one", new owned C(1));
  m.add("two", new owned C(2));

  var x = m.getValue("one");
  x!.i = -1;

  writeln(m);
}
test();

