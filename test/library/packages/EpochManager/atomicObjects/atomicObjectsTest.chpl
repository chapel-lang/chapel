use AtomicObjects;

class C {
  var x : int;
}

proc main() {
  var x = new unmanaged C(1);
  var atomicObj = new AtomicObject(unmanaged C, hasABASupport=true);
  atomicObj.write(x);
  var y = atomicObj.read();
  writeln(y);
  var z = atomicObj.readABA();
  writeln(z);
  var w = new unmanaged C(2);
  writeln(atomicObj.compareExchange(x, z.getObject()));
  writeln(atomicObj.read());
  writeln(atomicObj.readABA());
  writeln(atomicObj.compareExchangeABA(z, w));
  writeln(atomicObj.read());
  writeln(atomicObj.readABA());
  writeln(atomicObj.compareExchange(w, x));
  writeln(atomicObj.read());
  writeln(atomicObj.readABA());
  writeln(atomicObj.exchange(nil));
  writeln(atomicObj.read());
  writeln(atomicObj.readABA());

  writeln();

  var a = atomicObj.readABA();
  var b = atomicObj.readABA();

  writeln(a==b);
  atomicObj.compareExchangeABA(a, x);
  writeln(a==b);

  var c = atomicObj.readABA();

  writeln(a==c);

  atomicObj.exchangeABA(c.getObject());
  var currABACnt = atomicObj.readABA().getABACount();
  forall 1 .. 1024 * 1024 {
    assert(c != atomicObj.exchangeABA(c.getObject()), "Failed assertion for comparison after exchangeABA");
  }
  var deltaABACnt = atomicObj.readABA().getABACount() - currABACnt;
  writeln(deltaABACnt);
  assert(deltaABACnt == 1024 * 1024);
}
