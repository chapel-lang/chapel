proc p() {
  class C { var x=3; }
  return new C();
}

const c = p();
writeln(c.x);
writeln(c);
