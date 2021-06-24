use OrderedMap;



class C {
  var i: int;
}

var m = new orderedMap(string, shared C, false, defaultComparator);

m.add("one", new shared C(1));
m.add("two", new shared C(2));

var x = m.getBorrowed("one");
writeln(x.type:string);
writeln(x);
x.i = -1;

writeln(m);
