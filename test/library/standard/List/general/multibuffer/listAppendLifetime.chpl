import List.list;

class C { var x: int = 0; }

var lstGlobal: list(borrowed C);

proc test1() {
  var lst: list(borrowed C);
  {
    var x = new borrowed C(128);
    // Should emit a lifetime checker error.
    lst.append(x);
  }
  return;
}
test1();

proc test2() {
  var x = new borrowed C(256);
  // Should emit a lifetime checker error.
  lstGlobal.append(x);
  return;
}
test2();

