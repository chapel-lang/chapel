use Set;

record r {
  var x = 0;
  proc init() { writeln('default init'); }
  proc init=(other: r) { writeln('init='); }
  proc deinit() { writeln('deinit'); }
}

operator r.=(ref lhs: r, rhs: r) { writeln('assign'); }

proc test() {
  var st = new set(r);
  st.add(new r());
}
test();

