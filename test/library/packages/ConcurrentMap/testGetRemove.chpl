use ConcurrentMap;

class C {
  var i: int;
}

var m = new shared ConcurrentMap(string, shared C);
var a = new C(1);
var b = new C(2);
m.add("one", a);
m.add("two", b);

var x = m.getAndRemove("one");

writeln(x);
writeln(m);
