config const l  = numLocales - 1;
config const rl = 0;

on Locales(l) {
  extern class C { var x: int; }

  proc my_foo(c: C, x: int) {
    c.x = x;
  }

  var myC = new C(5);

  writeln(myC);

  on Locales(rl) {
    my_foo(myC, 3);
  }

  writeln(myC);

  delete myC;
}

