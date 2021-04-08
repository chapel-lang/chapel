use Set;

record r {
  var doPrint = false;
  var x = 0;

  proc init() { if doPrint then writeln('default init'); }
  proc init(id: int, doPrint: bool=false) {
    this.doPrint = doPrint;
    this.x = id;
    this.complete();
    if doPrint then writeln('init int');
  }
  proc init=(rhs: r) {
    this.doPrint = rhs.doPrint;
    this.x = rhs.x;
    this.complete();
    if doPrint then writeln('init=');
  }
  proc deinit() { if doPrint then writeln('deinit'); }
}

operator =(ref lhs: r, rhs: r) { writeln('assign'); }

// Test migrating to N>=12 locales then adding to a set on Locale 0.
// N>=12 is the number of distributed set.add() calls before a
// segfault occurs in #17214.
proc test1() {
  writeln('T1');

  var st = new set(r);

  // Interested in the number of copies made here.
  for loc in Locales do on loc {
    var x = new r(here.id);
    st.add(x);
  }

  for loc in Locales do on loc {
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

// Test local and remote add.
proc test3() {
  writeln('T3');

  var st = new set(r);

  // Local add...
  st.add(new r(here.id, doPrint=true));

  // Remote add...
  on Locales[1] {
    st.add(new r(here.id, doPrint=true));
  }

  assert(st.size == 2);
}
test3();

