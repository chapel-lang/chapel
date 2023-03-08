use Map;

class C {
  var i: int;
}

var m: map(string, shared C);
m.add("one", new shared C(1));
m.add("two", new shared C(2));

var sentinel = new shared C(3);
var x = m.get("one", sentinel);
m.remove("one");

writeln(x);
writeln(m);
