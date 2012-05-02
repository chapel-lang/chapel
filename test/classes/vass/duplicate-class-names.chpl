proc p {
  class C { var x=3; }
  return new C();
}
proc q {
  class C { var y=8; }
  return new C();
}
class C { var z=17; }
proc r {
  return new C();
}
writeln(p);
writeln(q);
writeln(r);
