use Set;

class C { var x = 0; }

record r {
  var c = new shared C(-1);

  proc init(id: int) {
    this.c = new shared C(id);
  }
}

// There isn't a public interface for this yet, but we need to override the
// hash function for 'r' in order to avoid hashing on the address contained
// in 'c', which will vary from instance to instance.
proc chpl__defaultHash(x: r) {
  return chpl__defaultHash(x.c.x);
}

operator r.==(lhs: r, rhs: r) {
  writeln('r==');
  return lhs.c.x == rhs.c.x;
}

proc test() {
  var st = new set(r);
  st.add(new r(1));
  assert(st.contains(new r(1)));
}
test();

