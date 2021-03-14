use myrecord;

proc returnRefArgBlank(arg) {
  return arg;
}
proc returnRefArgConstRef(const ref arg) {
  return arg;
}
proc returnRefArgRef(ref arg) {
  return arg;
}
proc returnRefArgBlankT(arg:R):R {
  return arg;
}
proc returnRefArgConstRefT(const ref arg:R):R {
  return arg;
}
proc returnRefArgRefT(ref arg:R):R {
  return arg;
}

proc returnRefArgBlankI(arg) {
  const ref tmp = arg;
  return tmp;
}
proc returnRefArgConstRefI(const ref arg) {
  const ref tmp = arg;
  return tmp;
}
proc returnRefArgRefI(ref arg) {
  ref tmp = arg;
  return tmp;
}

proc myfunction() {
  var r: R;
  r.setup(x = 1);
  r.verify();

  var a = returnRefArgBlank(r);
  a.verify();
  var b = returnRefArgConstRef(r);
  b.verify();
  var c = returnRefArgRef(r);
  c.verify();
  
  var d = returnRefArgBlankT(r);
  d.verify();
  var e = returnRefArgConstRefT(r);
  e.verify();
  var f = returnRefArgRefT(r);
  f.verify();

  var g = returnRefArgBlankI(r);
  g.verify();
  var h = returnRefArgConstRefI(r);
  h.verify();
  var i = returnRefArgRefI(r);
  i.verify();
}

myfunction();

verify();

