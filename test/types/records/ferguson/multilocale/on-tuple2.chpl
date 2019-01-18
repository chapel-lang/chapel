use myrecord;

proc myfunction() {

  var tup: (R,R);
  tup(1).setup(x = 20);
  tup(1).verify();
  tup(2).setup(x = 40);
  tup(2).verify();

  on Locales[numLocales-1] {
    var locTup = tup;
    locTup(1).verify();
    locTup(2).verify();
    assert(locTup(1).x == 20);
    assert(locTup(2).x == 40);
  }
}

myfunction();

verify();

