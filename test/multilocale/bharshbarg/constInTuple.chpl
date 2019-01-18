
proc helper(const in idx:3*int) {
  assert(idx.locale == here);
  writeln("Got ", idx);
}

inline proc wrapper(idx:3*int) {
  helper(idx);
}

proc main() {
  var foo = (1,2,3);

  on Locales[numLocales-1] {
    assert(foo.locale != here);
    wrapper(foo);
  }
}
