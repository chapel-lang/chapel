use myrecord;

proc myfunction() {

  var tup: (R,R);

  on Locales[numLocales-1] {
    var localTup: (R,R);
    
    localTup(0).setup(x = 50);
    localTup(0).verify();
    localTup(1).setup(x = 60);
    localTup(1).verify();

    tup = localTup;

    /*
    tup(0).verify();
    assert(tup(0).x == 50);
    tup(1).verify();
    assert(tup(1).x == 60);*/
  }

  tup(0).verify();
  assert(tup(0).x == 50);
  tup(1).verify();
  assert(tup(1).x == 60);
}

myfunction();

verify();

