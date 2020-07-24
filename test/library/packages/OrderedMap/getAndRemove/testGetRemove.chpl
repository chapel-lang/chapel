use OrderedMap;

config param impl: mapImpl;

class C {
  var i: int;
}

var m= new orderedMap(string, shared C, false, defaultComparator, impl);
m.add("one", new shared C(1));
m.add("two", new shared C(2));

var x = m.getAndRemove("one");

writeln(x);
writeln(m);
