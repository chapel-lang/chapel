use myrecord;

proc myfunction() {

  var tup: (R,R);
  tup(0).setup(x = 20);
  tup(0).verify();
  tup(1).setup(x = 40);
  tup(1).verify();

  on Locales[numLocales-1] {
    var locTup = tup;
    locTup(0).verify();
    locTup(1).verify();
    assert(locTup(0).x == 20);
    assert(locTup(1).x == 40);
  }
}

myfunction();

verify();

