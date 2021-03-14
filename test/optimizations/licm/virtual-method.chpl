class D {
  proc fn() : (bool, int) {
    halt("Should not be called...");
  }
}

class C : D {
  override proc fn() : (bool, int) {
    return (false, 1);
  }
}

var c : unmanaged D = new unmanaged C();

var (a, b) = (true, 0);
for 1 .. 5 {
  if b != 0 then writeln("a: ", a, ", b: ", b);
  (a, b) = c.fn();
}

delete c;
