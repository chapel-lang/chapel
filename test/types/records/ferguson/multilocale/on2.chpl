use myrecord;

proc myfunction() {

  var rec: R;
  rec.setup(x = 20);

  rec.verify();

  on Locales[numLocales-1] {
    var localRec = rec;

    localRec.verify();
    assert(localRec.x == 20);
  }
}

myfunction();

verify();

