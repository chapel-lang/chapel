import List.list;

class C { var x: int = 0; }

proc test() {
  var arr: [1..0] borrowed C?;
  {
    var x = new borrowed C?(128);
    // Should emit a lifetime checker error.
    arr.push_back(x);
  }
  return;
}
test();

