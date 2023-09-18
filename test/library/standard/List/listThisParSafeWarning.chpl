use List;

proc test() {
  var lst = new list(int, parSafe=true);
  for i in 0..3 do lst.pushBack(i);
  for i in 0..3 {
    ref v = lst[i];
    writeln(v);
  }
}
test();
