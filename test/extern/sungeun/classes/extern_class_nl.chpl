config const l  = numLocales - 1;
config const rl = 0;

on Locales(l) {
  extern class C { var x: int(64); }
  var myC = new C(5);

  writeln(myC);

  on Locales(rl) {
    myC.x = 3;
  }

  writeln(myC);

  delete myC;
}

