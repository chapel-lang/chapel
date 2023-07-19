use CTypes;

extern record MyRec {
  var x: c_ptr(void);
}

record MyChplRec {
  var x: c_ptr(void);
}

extern proc getMyRec(): MyRec;
extern proc takeFuncPtr(x: c_ptr(void));

var rec = getMyRec();
var otherRec = new MyChplRec(rec.x);
takeFuncPtr(otherRec.x);
