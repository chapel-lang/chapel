// Which combinations of writeArr* and addEm* should be allowed?

proc writeArrRef(ref X) {
  writeln(X);
}
proc writeArrCR(const ref X) {
  writeln(X);
}

proc addEmVal() {
  return [1,2];
}
proc addEmConst() const {
  return [1,2];
}

writeArrRef(addEmVal());
writeArrCR(addEmVal());
writeArrRef(addEmConst());
writeArrCR(addEmConst());
