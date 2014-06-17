class C {
  proc foo() {
    writeln((here.id, "C"));
  }
}

class C0: C {
  proc foo() {
    writeln((here.id, "C0"));
  }
}

class C1: C {
  proc foo() {
    writeln((here.id, "C1"));
  }
}

proc blah(c) {
  c.foo();
}

var c = new C();
var c0 = new C0();
var c1 = new C1();

for loc in Locales do on loc {
  blah(c);
  blah(c0);
  blah(c1);
}
