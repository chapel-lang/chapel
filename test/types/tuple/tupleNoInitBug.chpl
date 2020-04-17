class C { var x: int = 0; }

proc test() {
  pragma "no init"
  var tup: (int, shared C);
  return;
}
test();
