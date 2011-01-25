class C {
  var x: int = 3;
}

proc bar() {
  var x: int = 2;
  proc foo(c: C) {
    writeln((c, x));
    delete c;
  }
  foo(new C(x));
}

bar();
