use Set;

record r {
  var x = 0;
  proc init() { writeln('default init'); }
  proc init(id: int) { x = id; writeln('init int'); }
  proc init=(other: r) { writeln('init='); }
  proc deinit() { writeln('deinit'); }
}

proc =(ref lhs: r, rhs: r) { writeln('assign'); }

// Test migrating to N>=12 locales then adding to a set on Locale 0.
// N>=12 is the number of distributed set.add() calls before a
// segfault occurs in #17214.
proc test1() {
  writeln('T1');

  var st = new set(r);

  // Interested in the number of copies made here.
  writeln('adding');
  for loc in Locales do on loc {
    writeln('loc=', here.id);
    var x = new r(here.id);
    st.add(x);
  }

  writeln('contains');
  for loc in Locales do on loc {
    writeln('loc=', here.id);
    var x = new r(here.id);
    assert(st.contains(x));
    st.remove(x);
  }

  assert(st.size == 0);
}
test1();

// Coforalls, locking set containing int.
proc test2() {
  writeln('T2');
  var st = new set(int, parSafe=true);

  coforall loc in Locales with (ref st) do on loc {
    st.add(here.id);
  }

  coforall loc in Locales with (ref st) do on loc {
    assert(st.contains(here.id));
    st.remove(here.id);
  }

  assert(st.size == 0);
}
test2();

