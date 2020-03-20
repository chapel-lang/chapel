use Map;

class C {
  var i: int;
}

var m: map(string, shared C?);

m.add("one", new shared C?(1));
m.add("two", new shared C?(2));

ref x = m.getReferenceToElt("one");
x!.i = -1;

writeln(m);
