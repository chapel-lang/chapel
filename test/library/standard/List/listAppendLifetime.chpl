import List.list;

class C { var x: int = 0; }

proc test() {
  var lst: list(borrowed C);
  {
    var x = new borrowed C(128);
    // Should emit a lifetime checker error.
    lst.append(x);
    writeln(lst);
  }
  return;
}
test();

