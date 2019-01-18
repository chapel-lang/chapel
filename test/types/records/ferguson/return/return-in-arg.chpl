use myrecord;

proc returnInArg(in arg) {
  return arg;
}
proc returnConstInArg(const in arg) {
  return arg;
}
proc returnInArgT(in arg:R):R {
  return arg;
}
proc returnConstInArgT(const in arg:R):R {
  return arg;
}

proc myfunction() {
  var r: R;
  r.setup(x = 1);
  r.verify();

  var a = returnInArg(r);
  a.verify();
  var b = returnConstInArg(r);
  b.verify();

  var c = returnInArgT(r);
  c.verify();
  var d = returnConstInArgT(r);
  d.verify();
}

myfunction();

verify();

