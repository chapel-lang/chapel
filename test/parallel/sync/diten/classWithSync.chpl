class C {
  var s: sync bool;
}

var c = new unmanaged C();
c.s.writeEF(true);
c.s.readFE();

if (c.s.isFull) then halt("Should never get here.");

delete c;
