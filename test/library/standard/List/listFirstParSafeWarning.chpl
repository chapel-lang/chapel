use List;

proc test() {
  var lst = new list(int, parSafe=true);
  lst.append(0);
  ref first = lst.first();
  writeln(first);
}
test();

