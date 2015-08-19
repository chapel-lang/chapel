use myrecord;

proc myfunction() {

  var rec: R;
  rec.init(x = 20);

  rec.verify();

  on Locales[1] {
    var localRec : R = rec;

    localRec.verify();
    assert(localRec.x == 20);
  }
}

myfunction();

verify();

