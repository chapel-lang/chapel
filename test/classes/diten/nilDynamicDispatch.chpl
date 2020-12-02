class C {
  proc foo() {
    writeln("C.foo()");
  }
}

class D:C {
  override proc foo() {
    writeln("D.foo()");
  }
}

pragma "unsafe" var ARR: [LocaleSpace] borrowed C;
var c = ARR[0]; // nil of non-nilablel type, due to pragma "unsafe"

c.foo();
