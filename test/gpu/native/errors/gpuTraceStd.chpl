use MyMod;

record myRecord {}

pragma "no gpu codegen"
proc convertToString(r: myRecord) {
  return "{}";
}

proc main() {
  badLoop(myRecord);
}
