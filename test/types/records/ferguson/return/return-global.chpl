use myrecord;

var globalR: R;

proc returnGlobal() {
  return globalR;
}

proc myfunction() {
  globalR.setup(x = 1);
  globalR.verify();

  var y = returnGlobal();
  y.verify();
}

myfunction();

globalR.destroy();

verify();
