use OrderedMap;

record C {
  var i: int;
}

var m = new orderedMap(string, C, false, defaultComparator);

m.add("one", new C(1));
m.add("two", new C(2));

ref x = m.getReference("one");
x.i = -1;

writeln(m);
