record r { var x: int = 0; }

record myMap {
  type K;
  type V;
  var k1: K;
  var v1: V;

  proc set(in k: K, in v: V) {
    k1 = k;
    v1 = v;
  }

  // TODO: It is not currently possible to yield e.g. key by const ref and
  // value by ref. That would probably require setitional syntax, among
  // other things.
  iter items() const ref {
    yield (k1, v1);
  }
}

proc test1() {
  var m = new myMap(int, r);
  m.set(128, new r(128));
  for tup in m.items() {
    writeln(tup);
    m.set(256, new r(256));
    writeln(tup);
  }
}
test1();

proc test2() {
  var m = new myMap(int, r);
  m.set(128, new r(128));
  for (k, v) in m.items() {
    writeln(k);
    writeln(v);
    m.set(256, new r(256));
    writeln(k);
    writeln(v);
  }
}
test2();

