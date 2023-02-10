use Map;

class C {
  var i: int;
}

var m: map(string, shared C);
m.add("one", new shared C(1));
m.add("two", new shared C(2));

// x is borrowed from m
var x = m["one"];

writeln(x);
// x now invaild
m.remove("one");
writeln(m);
