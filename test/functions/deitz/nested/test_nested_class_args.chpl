class C {
  var x: int = 3;
}

proc bar() {
  var x: int = 2;
  proc foo(c: unmanaged C) {
    writeln((c, x));
    delete c;
  }
  foo(new unmanaged C(x));
}

bar();
