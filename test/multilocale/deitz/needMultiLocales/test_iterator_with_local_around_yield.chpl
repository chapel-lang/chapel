iter ii() {
  for i in 1..10 {
    local {
      yield i;
    }
  }
}

class C {
  var i: int = -1;
}

var c = new unmanaged C();

on Locales(1) {
  for i in ii() {
    writeln(c.i);
  }
}

delete c;
