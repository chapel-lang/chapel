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
  writeln(atomicObj.compareAndSwap(x, z.getObject()));
  writeln(atomicObj.read());
  writeln(atomicObj.readABA());
  writeln(atomicObj.compareAndSwapABA(z, w));
  writeln(atomicObj.read());
  writeln(atomicObj.readABA());
  writeln(atomicObj.compareAndSwap(w, x));
  writeln(atomicObj.read());
  writeln(atomicObj.readABA());
  writeln(atomicObj.exchange(nil));
  writeln(atomicObj.read());
  writeln(atomicObj.readABA());

  writeln();

  var a = atomicObj.readABA();
  var b = atomicObj.readABA();

  writeln(a==b);
  atomicObj.compareAndSwapABA(a, x);
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

  writeln();

  // writeABA(obj) stores obj and bumps the ABA count
  var d = atomicObj.readABA();
  atomicObj.writeABA(w);
  var e = atomicObj.readABA();
  writeln(e.getObject() == w);
  writeln(e.getABACount() == d.getABACount() + 1);

  // writeABA(ABA) stores the ABA as-is
  atomicObj.writeABA(d);
  writeln(atomicObj.readABA() == d);

  // compareAndSwapABA(ABA, ABA)
  var f = atomicObj.readABA();
  writeln(atomicObj.compareAndSwapABA(f, e));
  writeln(atomicObj.read() == w);
  writeln(atomicObj.readABA().getABACount() == f.getABACount() + 1);
  writeln(atomicObj.compareAndSwapABA(f, e));
  writeln(atomicObj.read() == w);

  delete x;
  delete w;
}
