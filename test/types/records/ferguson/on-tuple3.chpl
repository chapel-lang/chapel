use myrecord;

proc myfunction() {

  var tup: (R,R);

  on Locales[numLocales-1] {
    var localTup: (R,R);
    
    localTup(1).init(x = 50);
    localTup(1).verify();
    localTup(2).init(x = 60);
    localTup(2).verify();

    tup = localTup;

    /*
    tup(1).verify();
    assert(tup(1).x == 50);
    tup(2).verify();
    assert(tup(2).x == 60);*/
  }

  tup(1).verify();
  assert(tup(1).x == 50);
  tup(2).verify();
  assert(tup(2).x == 60);
}

myfunction();

verify();

