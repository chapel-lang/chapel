// Use non-prototype module to ensure strict nilability checking.
module w {

  class CC {
    param pfield;
    type  nfield;  // non-runtime type
    type  rfield;  // runtime type
  }

  // a runtime type
  type RT = [Locales.domain] int;

  proc test(ref arg) {
    type AT = arg.type;
    compilerWarning("==== ", AT:string);
    compilerWarning(arg.pfield);
    compilerWarning(AT.pfield);
    compilerWarning(arg.nfield:string);
    compilerWarning(AT.nfield:string);
    if !isNilableClass(arg) {
      var arr: arg.rfield;
      writeln(arr.dom == Locales.dom);
      // Do not access AT.rfield -- see #11549 and with-runtime-types.chpl
    }
  }

  var db = new           CC("default",    int, RT);   test(db);
  var dq = new           CC("default?",   int, RT)?;  test(dq);
  var ob = new owned     CC("owned",      int, RT);   test(ob);
  var oq = new owned     CC("owned?",     int, RT)?;  test(oq);
  var sb = new shared    CC("shared",     int, RT);   test(sb);
  var sq = new shared    CC("shared?",    int, RT)?;  test(sq);
  var bb = new borrowed  CC("borrowed",   int, RT);   test(bb);
  var bq = new borrowed  CC("borrowed?",  int, RT)?;  test(bq);
  var ub = new unmanaged CC("unmanaged",  int, RT);   test(ub);  delete ub;
  var uq = new unmanaged CC("unmanaged?", int, RT)?;  test(uq);  delete uq;

  compilerWarning("=== done", 0);
  writeln("done");
}
