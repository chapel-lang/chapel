use MyMod;

record myRecord {}

pragma "no gpu codegen"
proc convertToString(r: myRecord) {
  return "{}";
}

proc notEligible() {
  if usePoi {
    doSomethingPOI(new myRecord());
  } else {
    doSomething(new myRecord());
  }
}

proc main() {
  @assertOnGpu
  foreach 1..10 do notEligible();
}
