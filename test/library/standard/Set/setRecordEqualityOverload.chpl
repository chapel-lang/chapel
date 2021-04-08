use Set;

class C { var x = 0; }

record r {
  var c = new shared C(-1);

  proc init(id: int) {
    this.c = new shared C(id);
  }

  operator ==(lhs: r, rhs: r) {
    writeln('r==');
    return lhs.c.x == rhs.c.x;
  }
}

proc test() {
  var st = new set(r);
  st.add(new r(1));
  assert(st.contains(new r(1)));
}
test();

