extern record MyRec {
  var x: c_void_ptr;
}

record MyChplRec {
  var x: c_void_ptr;
}

extern proc getMyRec(): MyRec;
extern proc takeFuncPtr(x: c_void_ptr);

var rec = getMyRec();
var otherRec = new MyChplRec(rec.x);
takeFuncPtr(otherRec.x);
