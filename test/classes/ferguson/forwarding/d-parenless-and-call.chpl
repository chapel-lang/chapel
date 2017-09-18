class Inner {
  var x: int;
  proc this(y:int) {
    writeln("In Inner(", x, ").this(", y, ")");
  }
}

class C {
  forwarding var inner;
  proc getit {
    return inner;
  }
}

proc test() {
  var ii = new Inner(1);
  var c = new C(ii);
  c.getit(2);
  delete c;
  delete ii;
}


test();
