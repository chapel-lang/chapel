use Set;

// Test migrating to N>=12 locales then adding to a set on Locale 0.
// N>=12 is the number of distributed set.add() calls before a
// segfault occurs in #17214.
proc test1() {
  var st = new set(int);

  for loc in Locales do on loc {
    st.add(here.id);
  }

  for loc in Locales do on loc {
    assert(st.contains(here.id));
    st.remove(here.id);
  }

  assert(st.size == 0);
}
test1();

// Same test, but parallel.
proc test2() {
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

