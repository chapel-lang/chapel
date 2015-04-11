use myrecord;

proc myfunction() {

  var rec: R;
  rec.init(x = 20);
  rec.verify();

  sync {
    begin {
      var localR = rec;
      localR.verify();
      assert(localR.x == 20);
    }
  }

}

myfunction();

