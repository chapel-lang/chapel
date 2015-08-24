use myrecord;

proc myfunction() {

  var rec: R;
  rec.init(x = 20);

  rec.verify();

  const const_copy = rec;

  on Locales[numLocales-1] {
    const_copy.verify();
    assert(rec.x == 20);
  }
}

myfunction();

verify();

