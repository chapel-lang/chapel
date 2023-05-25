class C {
  proc foo() {
    writeln((here.id, "C"));
  }
}

class C0: C {
  override proc foo() {
    writeln((here.id, "C0"));
  }
}

class C1: C {
  override proc foo() {
    writeln((here.id, "C1"));
  }
}

proc blah(c) {
  c.foo();
}

var c = (new owned C()).borrow();
var c0 = (new owned C0()).borrow();
var c1 = (new owned C1()).borrow();

for loc in Locales do on loc {
  blah(c);
  blah(c0);
  blah(c1);
}
