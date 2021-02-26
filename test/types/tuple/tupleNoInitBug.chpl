class C { var x: int = 0; }

proc keepArgAlive(tup) {
  return;
}

proc test() {
  pragma "no init"
  var tup: (int, shared C);
  keepArgAlive(tup);
  return;
}
test();
