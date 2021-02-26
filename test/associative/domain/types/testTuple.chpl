class C { var x: int = 0; }

proc test() {
  type T = (int, shared C);
  var dom: domain(T);
  var tup = (0, new shared C(64));
  dom.add(tup);
  writeln(dom);
  return;
}
test();

