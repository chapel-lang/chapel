use myrecord;

proc myfunction() {

  var tup: (R,R);
  tup(1).init(x = 20);
  tup(1).verify();
  tup(2).init(x = 40);
  tup(2).verify();

  on Locales[1] {
    var locTup = tup;
    locTup(1).verify();
    locTup(2).verify();
    assert(locTup(1).x == 20);
    assert(locTup(2).x == 40);
  }
}

myfunction();

verify();

