extern record MyRec {
  var x: c_fn_ptr;
}

record MyChplRec {
  var x: c_fn_ptr;
}

extern proc getMyRec(): MyRec;
extern proc takeFuncPtr(x: c_fn_ptr);

var rec = getMyRec();
var otherRec = new MyChplRec(rec.x);
takeFuncPtr(otherRec.x);
