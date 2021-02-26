proc p() {
  // Should this be allowed?  See #15818 for discussion on type uniqueness
  class C { var x=3; }
  return new C();
}

const c = p();
writeln(c.x);
writeln(c);
