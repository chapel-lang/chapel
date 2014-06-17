class C {
  var s: sync bool;
}

var c = new C();
c.s = true;
c.s;

if (c.s.isFull) then halt("Should never get here.");

delete c;
