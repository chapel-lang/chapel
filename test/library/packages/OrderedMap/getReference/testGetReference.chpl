use OrderedMap;

config param impl: mapImpl;

class C {
  var i: int;
}

var m= new orderedMap(string, shared C?, false, defaultComparator, impl);

m.add("one", new shared C?(1));
m.add("two", new shared C?(2));

ref x = m.getReference("one");
x!.i = -1;

writeln(m);
