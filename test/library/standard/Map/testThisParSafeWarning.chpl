use Map;

record r { var x: int = 0; }

proc test() {
  var m = new map(int, r, parSafe=true);
  m.add(0, new r());
  writeln(m);
  m[0] = new r(64);
  writeln(m);
}
test();

