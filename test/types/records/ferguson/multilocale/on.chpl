use myrecord;

proc myfunction() {

  var rec: R;
  rec.setup(x = 20);

  rec.verify();

  on Locales[numLocales-1] {
    rec.verify();
    assert(rec.x == 20);
  }
}

myfunction();

verify();

