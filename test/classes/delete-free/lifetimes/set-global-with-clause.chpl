class C { }

var globalC: borrowed C?;
var otherGlobalC: owned C?;

proc setGlobal1(arg: borrowed C?) lifetime arg > globalC {
  globalC = arg;
}
proc setGlobal2(arg: borrowed C?) lifetime globalC <= arg {
  globalC = arg;
}
proc setGlobal3(arg: borrowed C?) lifetime globalC = arg {
  globalC = arg;
}


proc test() {
  otherGlobalC = new owned C();
  setGlobal1(otherGlobalC);
  setGlobal2(otherGlobalC);
  setGlobal3(otherGlobalC);
}
test();
