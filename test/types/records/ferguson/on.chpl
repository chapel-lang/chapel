use myrecord;

proc myfunction() {

  var rec: R;
  rec.init(x = 20);

  rec.verify();

  on Locales[1] {
    // This should check that the C pointer
    // inside rec points to rec.x (on Locale 0)
    rec.verify();
    assert(rec.x == 20);
  }
}

myfunction();

verify();

