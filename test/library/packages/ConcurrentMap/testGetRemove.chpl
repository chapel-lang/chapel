use ConcurrentMap;

class C {
  var i: int;
}

var m = new shared ConcurrentMap(string, int);
var a = 1;
var b = 2;
m.add("one", a);
m.add("two", b);

var x = m.getAndRemove("one");

writeln(x);
writeln(m);
