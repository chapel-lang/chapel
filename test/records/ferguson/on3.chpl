use myrecord;

proc myfunction() {

  var rec: R;

  on Locales[1] {
    var localRec: R;
    
    localRec.init(x = 50);
    localRec.verify();

    rec = localRec;
  }

  rec.verify();
  assert(rec.x == 50);
}

myfunction();

