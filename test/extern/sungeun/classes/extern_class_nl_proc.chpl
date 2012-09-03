config const l = numLocales-1;
config const rl = 0;

on rootLocale.getLocales()(l) {
  extern class C { var x: int; }
  proc my_foo(c: C, x: int) {
    c.x = x;
  }
  var myC = new C(5);

  writeln(myC);

  on rootLocale.getLocales()(rl) {
    my_foo(myC, 3);
  }

  writeln(myC);
}

