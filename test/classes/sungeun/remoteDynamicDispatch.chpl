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

var cTmp = new owned C();
var c = cTmp.borrow();
var c0Tmp = new owned C0();
var c0 = c0Tmp.borrow();
var c1Tmp = new owned C1();
var c1 = c1Tmp.borrow();

for loc in Locales do on loc {
  blah(c);
  blah(c0);
  blah(c1);
}
