class C { var x: int; }

proc main() {
  compilerWarning(isNonNilableClass(unmanaged C):string);

  var x: unmanaged C = new unmanaged C(1);
  var v: sync unmanaged C = x;
  var y = v.readFE();
  assert(y == x);
  delete x;
}
