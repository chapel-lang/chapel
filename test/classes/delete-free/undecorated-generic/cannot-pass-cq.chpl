class C { var x: int; }

proc typefn(type t) { }
proc test() {
  typefn(C?);
}
test();
