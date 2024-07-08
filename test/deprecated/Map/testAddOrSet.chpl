use Map;

class C {
  var a: int;
}

var m: map(int, shared C);

var a = new shared C(1);

m.addOrSet(1, a);
m.addOrSet(2, m.getAndRemove(1));

writeln(m);
